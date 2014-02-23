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
#include "OCVCapture.h"

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

const unsigned long loopDelayTime = 10;
int run = 1;
int captureStill = 0;

/*
 * Set run to false on SIGINT
 */
void handleSigInt(int sig)
{
  printf("Caught signal %d, will exit.\n", sig);
  run = 0;
}

/*
 * Capture a frame on SIGUSR1
 */
void handleStillCapSig(int sig)
{
  printf("Caught signal %d, will capture still image.\n", sig);
  captureStill = 1;
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
      printf("BubbleScopeApp\n");
      printParameterUsage();
      return 0;
      break;
    default:    //Parameter error
      printf("Invalid parameters!\n");
      printParameterUsage();
      return 1;
  }

  //Tell the user how things are going to happen
  printParameters(&params);

  //Setup the image unwrapper
  BubbleScopeUnwrapper unwrapper;
  unwrapper.unwrapWidth(params.unwrapWidth);
  unwrapper.originalCentre(params.uCentre, params.vCentre);
  unwrapper.imageRadius(params.radiusMin, params.radiusMax);
  unwrapper.offsetAngle(params.offsetAngle);

  //Open the capture device and check it is working
  OCVCapture cap;
  cap.setVerbose(false);
  cap.setDesiredSize(params.originalWidth, params.originalHeight);
  cap.open(params.captureDevice.c_str());
  if(!cap.isOpen())
  {
    printf("Can't open video capture source!\n");
    return 2;
  }

  //After capture size is determined setup transofrmation array
  unwrapper.originalSize(cap.width(), cap.height());
  unwrapper.generateTransformation();

  //The container for captured frames
  cv::Mat frame;

  //Setup video output
  cv::VideoWriter videoOut;
  if(params.mode[MODE_VIDEO])
  {
    cv::Size videoSize = cv::Size(params.unwrapWidth, unwrapper.getUnwrapHeight());
    videoOut.open(params.outputFilename[MODE_VIDEO].c_str(), CV_FOURCC('M','J','P','G'), params.fps, videoSize, true);
    if(!videoOut.isOpened())
      printf("Can't open video output file! (will continue with capture)\n");
  }

  //Number of still frames already captures, used for filename formatting
  int stillFrameNumber = 0;

  //Stuff for measuring capture properites
  Timer fpsTimer;
  float measuredFPS = 0.0f;
  int capPropFrame = 0;

  printf("Starting capture.\n");
  while(run)
  {
    //Start FPS timer for capture properties
    if(params.showCaptureProps)
      fpsTimer.start();

    //Capture a frame
    cap.grab();
    cap.rgb(frame);

    //Unwrap it
    cv::Mat unwrap = unwrapper.unwrap(&frame);

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

    if(params.mode[MODE_SHOW_ORIGINAL] || params.mode[MODE_SHOW_UNWRAP])
    {
      char keyPress = cv::waitKey(loopDelayTime);
      switch(keyPress)
      {
        //Exit on 'q' or ESC
        case 'q':
        case 27:
          printf("Exiting.\n");
          run = 0;
          break;
        case ' ':
          captureStill = 1;
          break;
      }
    }
    else
      delay(loopDelayTime);
    

    if(params.mode[MODE_STILLS] && captureStill)
    {
      //Format filename with frame number
      int filenameLen = strlen(params.outputFilename[MODE_STILLS].c_str()) + 2;
      char stillFilename[filenameLen];
      sprintf(stillFilename, params.outputFilename[MODE_STILLS].c_str(), stillFrameNumber);
      printf("Saving still image: %s\n", stillFilename);
      //Save still image
      imwrite(stillFilename, unwrap);
      stillFrameNumber++;
      captureStill = 0;
    }

    if(params.showCaptureProps)
    {
      //Measure time for single frame
      fpsTimer.stop();
      //Calculate rolling FPS average
      float measuredFPS = 0.7 * (1000.0f / fpsTimer.getElapsedTimeInMilliSec()) + 0.3 * measuredFPS;

      //Show capture properties every 10 frames
      if(capPropFrame % 10 == 0)
      {
        printf("Average FPS: %f\n", measuredFPS);
        printf("Input image size: %dx%d\n", frame.cols, frame.rows);
        capPropFrame = 0;
      }
    }

    //Done a single capture, can now exit
    if(params.mode[MODE_SINGLE_STILL])
      run = 0;
  }

  return 0;
}
