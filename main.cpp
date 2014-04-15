/*
 * BubbleScope V4L2 capture app
 * Allows capturing videos and stills from a BubbleScope fitted V4L2 device.
 *
 * Dan Nixon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "bubblescope_capture_params.h"
#include "unwrap.h"
#include "command_line_params.h"
#include "Timer.h"

#include "frame_source/frame_source.h"
#include "frame_source/source_v4l2.h"
#include "frame_source/source_imagefile.h"
#include "frame_source/source_videofile.h"
#include "frame_source/source_timelapse.h"

//Cross platform delay, taken from: http://www.cplusplus.com/forum/unices/10491/
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
  #include <windows.h>
  inline void delay(unsigned long ms)
  {
    Sleep(ms);
  }
#else
  #include <unistd.h>
  inline void delay(unsigned long ms)
  {
    usleep(ms * 1000);
  }
#endif 

unsigned long loopDelayTime = 10;

bool run = 1;
bool captureStill = 0;

/*
 * Set run to false on SIGINT
 */
void handleSigInt(int sig)
{
  printf("Caught signal %d, will exit.\n", sig);
  run = false;
}

/*
 * Capture a frame on SIGUSR1
 */
void handleStillCapSig(int sig)
{
  printf("Caught signal %d, will capture still image.\n", sig);
  captureStill = true;
}

int main(int argc, char **argv)
{
  //Setup SIGINT handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = handleSigInt;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  //Setup SIGUSR1 handler
  struct sigaction stillCapSigHandler;
  stillCapSigHandler.sa_handler = handleStillCapSig;
  sigemptyset(&stillCapSigHandler.sa_mask);
  stillCapSigHandler.sa_flags = 0;
  sigaction(SIGUSR1, &stillCapSigHandler, NULL);

  //Get some storage for parameters
  BubbleScopeParameters params;
  setupDefaultParameters(&params);

  //Get parameters
  switch(getParameters(&params, argc, argv))
  {
    case 0:     //All is good, carry on
      break;
    case HELP:  //User wants help
      printf("BubbleScopeCL\n");
      printf("See https://github.com/DanNixon/BubbleScopeCL for more info.\n");
      printParameterUsage();
      return 0;
      break;
    default:    //Parameter error
      printf("Invalid parameters!\n");
      printf("See https://github.com/DanNixon/BubbleScopeCL for more info.\n");
      printParameterUsage();
      return 1;
  }

  //Get the correct capture source and open it
  FrameSource *cap;
  switch(params.captureSource)
  {
    case SOURCE_V4L2:
      cap = new V4L2Source();
      dynamic_cast<V4L2Source *>(cap)->setCaptureSize(params.originalWidth, params.originalHeight);
      break;
    case SOURCE_VIDEO:
      cap = new VideoFileSource();
      params.unwrapCapture = true;
      loopDelayTime = 0;
      break;
    case SOURCE_STILL:
      cap = new ImageFileSource();
      params.unwrapCapture = true;
      loopDelayTime = 0;
      break;
    case SOURCE_TIMELAPSE:
      cap = new TimelapseSource();
      params.unwrapCapture = true;
      loopDelayTime = 0;
      break;
  }
  cap->open(params.captureLocation);

  //Update some vars for user feedback
  params.originalWidth = cap->getWidth();
  params.originalHeight = cap->getHeight();

  //Setup the image unwrapper
  BubbleScopeUnwrapper unwrapper;
  if(params.unwrapCapture)
  {
    unwrapper.unwrapWidth(params.unwrapWidth);
    unwrapper.originalCentre(params.uCentre, params.vCentre);
    unwrapper.imageRadius(params.radiusMin, params.radiusMax);
    unwrapper.offsetAngle(params.offsetAngle);
  }

  //Check capture is working
  if(!cap->isOpen())
  {
    printf("Can't open image capture source!\n");
    return 2;
  }

  //Get the input video frame rate if used
  if(params.captureSource == SOURCE_VIDEO)
    params.fps = dynamic_cast<VideoFileSource *>(cap)->getFrameRate();

  if(params.unwrapCapture)
  {
    //After capture size is determined setup transofrmation array
    unwrapper.originalSize(cap->getWidth(), cap->getHeight());
    unwrapper.generateTransformation();
  }

  //The container for captured frames
  cv::Mat frame;

  if(params.sampleFPS && params.mode[MODE_VIDEO] && params.captureSource == SOURCE_V4L2)
  {
    printf("Measuring V4L2 capture frame rate over %d frames...\n", params.sampleFPS);
    unsigned int frames = 0;
    Timer fpsSampleTimer;
    fpsSampleTimer.start();
    while(frames < params.sampleFPS)
    {
      cap->grab(&frame);
      delay(loopDelayTime);
      frames++;
    }
    fpsSampleTimer.stop();
    double measuredFPS = (double) params.sampleFPS / (fpsSampleTimer.getElapsedTimeInMilliSec() / 1000.0f);
    printf("Measured %f FPS\n", measuredFPS);
    params.fps = measuredFPS;
  }

  //Setup video output
  cv::VideoWriter videoOut;
  if(params.mode[MODE_VIDEO])
  {
    cv::Size videoSize;
    if(params.unwrapCapture)
      videoSize = cv::Size(params.unwrapWidth, unwrapper.getUnwrapHeight());
    else
      videoSize = cv::Size(params.originalWidth, params.originalHeight);
    videoOut.open(params.outputFilename[MODE_VIDEO].c_str(), CV_FOURCC('M','J','P','G'), params.fps, videoSize, true);
    if(!videoOut.isOpened())
      printf("Can't open video output file! (will continue with capture)\n");
  }

  //Timelaspe capture timer
  Timer *timelapseTimer;

  //Tell the user how things are going to happen
  printf("Capture parameters:\n");
  printParameters(&params);
  printf("\n");

  //Number of still frames already captures, used for filename formatting
  unsigned long stillFrameNumber = 0;
  unsigned long timelapseFrameNumber = 0;

  printf("Starting capture/conversion.\n");

  //Start timelapse timing
  if(params.mode[MODE_TIMELAPSE])
  {
    timelapseTimer = new Timer();
    timelapseTimer->start();
  }

  while(run)
  {
    //Try to capture a frame
    if(!cap->grab(&frame))
    {
      printf("Capture returned no frame, exiting.\n");
      run = 0;
      break;
    }

    //Unwrap it
    cv::Mat unwrap;
    if(params.unwrapCapture)
      unwrap = unwrapper.unwrap(&frame);
    else
      unwrap = frame;

    //Show the original if asked to
    if(params.mode[MODE_SHOW_ORIGINAL])
      imshow("BubbleScope Original Image", frame);

    //Show the unwrapped if asked to
    if(params.mode[MODE_SHOW_UNWRAP])
      imshow("BubbleScope Unwrapped Image", unwrap);
  
    //Record video if asked to
    if(params.mode[MODE_VIDEO])
      videoOut.write(unwrap);

    //Save an MJPG frame if asked to
    if(params.mode[MODE_MJPG] || params.mode[MODE_SINGLE_STILL])
      imwrite(params.outputFilename[MODE_MJPG], unwrap);

    //If time has elepsed save a timelapse frame
    if(params.mode[MODE_TIMELAPSE] &&
        ((timelapseTimer->getElapsedTimeInMilliSec() > params.mode[MODE_TIMELAPSE])
         || params.captureSource == SOURCE_TIMELAPSE))
    {
      //Format filename with frame number
      int filenameLen = strlen(params.outputFilename[MODE_TIMELAPSE].c_str()) + 5;
      char timelapseFilename[filenameLen];
      sprintf(timelapseFilename, params.outputFilename[MODE_TIMELAPSE].c_str(), timelapseFrameNumber);
      //Save timelapse frame
      imwrite(timelapseFilename, unwrap);
      timelapseFrameNumber++;
      //Restart timer
      timelapseTimer->stop();
      timelapseTimer = new Timer();
      timelapseTimer->start();
    }

    if(params.mode[MODE_SHOW_ORIGINAL] || params.mode[MODE_SHOW_UNWRAP])
    {
      char keyPress = cv::waitKey(loopDelayTime);
      switch(keyPress)
      {
        //Exit on 'q' or ESC
        case 'q':
        case 27:
          printf("Exiting.\n");
          run = false;
          break;
        case ' ':
          captureStill = true;
          break;
      }
    }
    else
      delay(loopDelayTime);
    
    //Handle specific capture loop exit conditions
    switch(params.captureSource)
    {
      case SOURCE_STILL:
        run = false;
        captureStill = true;
        break;
      case SOURCE_VIDEO:
        run = !dynamic_cast<VideoFileSource *>(cap)->atEndOfVideo();
        break;
      case SOURCE_V4L2:
        break;
    }

    if(params.mode[MODE_STILLS] && captureStill)
    {
      //Format filename with frame number
      int filenameLen = strlen(params.outputFilename[MODE_STILLS].c_str()) + 5;
      char stillFilename[filenameLen];
      sprintf(stillFilename, params.outputFilename[MODE_STILLS].c_str(), stillFrameNumber);
      printf("Saving still image: %s\n", stillFilename);
      //Save still image
      imwrite(stillFilename, unwrap);
      stillFrameNumber++;
      captureStill = false;
    }

    //Done a single capture, can now exit
    if(params.mode[MODE_SINGLE_STILL])
      run = false;
  }

  cap->close();

  printf("Complete.\n");
  return 0;
}
