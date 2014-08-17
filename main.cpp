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

#include "BubbleScopeUnwrapper.h"
#include "CommandLineParser.h"
#include "Configuration.h"
#include "Timer.h"

#include "FrameSource/FrameSource.h"
#include "FrameSource/V4L2Source.h"
#include "FrameSource/ImageFileSource.h"
#include "FrameSource/VideoFileSource.h"
#include "FrameSource/TimelapseSource.h"

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

  //Get command line parameters
  CommandLineParser parser;
  parser.parse(argc, argv);

  if(parser.helpWanted())
  {
    parser.printUsage(std::cout);
    return 0;
  }

  BubbleScopeCaptureParams captureParams = parser.getCaptureParams();
  BubbleScopeUnwrapParams unwrapParams = parser.getUnwrapParams();

  //TODO: Temp.
  captureParams.print(std::cout);
  unwrapParams.print(std::cout);

  //Get the correct capture source and open it
  FrameSource *cap;
  switch(captureParams.captureSource)
  {
    case SOURCE_V4L2:
      cap = new V4L2Source();
      dynamic_cast<V4L2Source *>(cap)->setCaptureSize(
          captureParams.originalWidth, captureParams.originalHeight);
      break;
    case SOURCE_VIDEO:
      cap = new VideoFileSource();
      captureParams.unwrapCapture = true;
      loopDelayTime = 0;
      break;
    case SOURCE_STILL:
      cap = new ImageFileSource();
      captureParams.unwrapCapture = true;
      loopDelayTime = 0;
      break;
    case SOURCE_TIMELAPSE:
      cap = new TimelapseSource();
      captureParams.unwrapCapture = true;
      loopDelayTime = 0;
      break;
  }
  cap->open(captureParams.captureLocation);

  //Get filename without type
  std::string::size_type dotIndex = captureParams.captureLocation.find('.');
  std::string filenameBase;
  if(dotIndex == std::string::npos)
    filenameBase = captureParams.captureLocation;
  else
    filenameBase = captureParams.captureLocation.substr(0, dotIndex);

  //Set default filenames
  if(captureParams.outputFilename[MODE_VIDEO] == "NONE")
    captureParams.outputFilename[MODE_VIDEO] = filenameBase + "_unwrap.mkv";
  if(captureParams.outputFilename[MODE_STILLS] == "NONE")
    captureParams.outputFilename[MODE_STILLS] = filenameBase + "_unwrap_%d.jpg";
  if(captureParams.outputFilename[MODE_TIMELAPSE] == "NONE")
    captureParams.outputFilename[MODE_TIMELAPSE] = filenameBase + "_unwrap_tl_%d.jpg";
  if(captureParams.outputFilename[MODE_MJPG] == "NONE")
    captureParams.outputFilename[MODE_MJPG] = filenameBase + "_unwrap_frame.jpg";

  //Dont show video display windows unless using V4L2
  if(captureParams.captureSource != SOURCE_V4L2)
  {
    captureParams.mode[MODE_SHOW_ORIGINAL] = 0;
    captureParams.mode[MODE_SHOW_UNWRAP] = 0;
    captureParams.mode[MODE_MJPG] = 0;
    captureParams.mode[MODE_VIDEO] = 0;
  }

  //Check capture is working
  if(!cap->isOpen())
  {
    printf("Can't open image capture source!\n");
    return 2;
  }

  //Update capture parameters with actual values
  captureParams.originalWidth = cap->getWidth();
  captureParams.originalHeight = cap->getHeight();

  //Setup the image unwrapper
  BubbleScopeUnwrapper unwrapper;
  if(captureParams.unwrapCapture)
  {
    unwrapper.unwrapWidth(unwrapParams.unwrapWidth);
    unwrapper.originalCentre(unwrapParams.uCentre, unwrapParams.vCentre);
    unwrapper.imageRadius(unwrapParams.radiusMin, unwrapParams.radiusMax);
    unwrapper.offsetAngle(unwrapParams.offsetAngle);
    unwrapper.originalSize(captureParams.originalWidth, captureParams.originalHeight);
    unwrapper.generateTransformation();
  }

  //Get the input video frame rate if used
  if(captureParams.captureSource == SOURCE_VIDEO)
    captureParams.fps = dynamic_cast<VideoFileSource *>(cap)->getFrameRate();

  //The container for captured frames
  cv::Mat frame;

  if(captureParams.sampleFPS &&
      captureParams.mode[MODE_VIDEO] &&
      captureParams.captureSource == SOURCE_V4L2)
  {
    printf("Measuring V4L2 capture frame rate over %d frames...\n",
        captureParams.sampleFPS);
    unsigned int frames = 0;
    Timer fpsSampleTimer;
    fpsSampleTimer.start();
    while(frames < captureParams.sampleFPS)
    {
      cap->grab(&frame);
      delay(loopDelayTime);
      frames++;
    }
    fpsSampleTimer.stop();
    double measuredFPS = (double) captureParams.sampleFPS /
      (fpsSampleTimer.getElapsedTimeInMilliSec() / 1000.0f);
    printf("Measured %f FPS\n", measuredFPS);
    captureParams.fps = measuredFPS;
  }

  if(captureParams.forceFPS > 0.0f)
    captureParams.fps = captureParams.forceFPS;

  //Setup video output
  cv::VideoWriter videoOut;
  if(captureParams.mode[MODE_VIDEO])
  {
    cv::Size videoSize;
    if(captureParams.unwrapCapture)
      videoSize = cv::Size(unwrapParams.unwrapWidth, unwrapper.getUnwrapHeight());
    else
      videoSize = cv::Size(captureParams.originalWidth, captureParams.originalHeight);
    videoOut.open(captureParams.outputFilename[MODE_VIDEO].c_str(),
        CV_FOURCC('M','J','P','G'), captureParams.fps, videoSize, true);
    if(!videoOut.isOpened())
      printf("Can't open video output file! (will continue with capture)\n");
  }

  //Timelaspe capture timer
  Timer *timelapseTimer;

  //Tell the user how things are going to happen
  captureParams.print(std::cout);
  unwrapParams.print(std::cout);

  //Save the config to file
  //TODO

  //Number of still frames already captures, used for filename formatting
  unsigned long stillFrameNumber = 0;
  unsigned long timelapseFrameNumber = 0;

  printf("Starting capture/conversion.\n");

  //Start timelapse timing
  if(captureParams.mode[MODE_TIMELAPSE])
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
    cv::Mat *unwrap;
    if(captureParams.unwrapCapture)
      unwrapper.unwrap(&frame, &unwrap);
    else
      unwrap = &frame;

    //Show the original image if asked to
    if(captureParams.mode[MODE_SHOW_ORIGINAL])
      imshow("BubbleScope Original Image", frame);

    //Show the unwrapped image if asked to
    if(captureParams.mode[MODE_SHOW_UNWRAP])
      imshow("BubbleScope Unwrapped Image", *unwrap);
  
    //Record video if asked to
    if(captureParams.mode[MODE_VIDEO])
      videoOut.write(*unwrap);

    //Save an MJPG frame if asked to
    if(captureParams.mode[MODE_MJPG] || captureParams.mode[MODE_SINGLE_STILL])
      imwrite(captureParams.outputFilename[MODE_MJPG], *unwrap);

    //If time has elepsed save a timelapse frame
    if(captureParams.mode[MODE_TIMELAPSE] &&
        ((timelapseTimer->getElapsedTimeInMilliSec() >
         captureParams.mode[MODE_TIMELAPSE])
         || captureParams.captureSource == SOURCE_TIMELAPSE))
    {
      //Format filename with frame number
      int filenameLen =
        strlen(captureParams.outputFilename[MODE_TIMELAPSE].c_str()) + 5;
      char timelapseFilename[filenameLen];
      sprintf(timelapseFilename, captureParams.outputFilename[MODE_TIMELAPSE].c_str(),
          timelapseFrameNumber);
      //Save timelapse frame
      imwrite(timelapseFilename, *unwrap);
      timelapseFrameNumber++;
      //Restart timer
      timelapseTimer->stop();
      timelapseTimer = new Timer();
      timelapseTimer->start();
    }

    if(captureParams.mode[MODE_SHOW_ORIGINAL] || captureParams.mode[MODE_SHOW_UNWRAP])
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
    switch(captureParams.captureSource)
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

    if(captureParams.mode[MODE_STILLS] && captureStill)
    {
      //Format filename with frame number
      int filenameLen = strlen(captureParams.outputFilename[MODE_STILLS].c_str()) + 5;
      char stillFilename[filenameLen];
      sprintf(stillFilename, captureParams.outputFilename[MODE_STILLS].c_str(),
          stillFrameNumber);
      printf("Saving still image: %s\n", stillFilename);
      //Save still image
      imwrite(stillFilename, *unwrap);
      stillFrameNumber++;
      captureStill = false;
    }

    //Done a single capture, can now exit
    if(captureParams.mode[MODE_SINGLE_STILL])
      run = false;
  }

  cap->close();

  printf("Complete.\n");
  return 0;
}
