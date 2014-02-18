/*
 * BubbleScope V4L2 capture app
 * Allows capturing videos and stills from a BubbleScope fitted V4L2 device.
 *
 * Dan Nixon
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "bubblescope_capture_params.h"
#include "unwrap.h"
#include "command_line_params.h"
#include "Timer.h"

int main(int argc, char **argv)
{
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
  cv::VideoCapture cap(params.captureDevice);
  if(!cap.isOpened())
  {
    printf("Can't open video capture source!\n");
    return 2;
  }

  cap.set(CV_CAP_PROP_FPS, params.fps);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, params.originalWidth);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, params.originalHeight);

  //The container for captured frames
  cv::Mat frame;

  //Capture an initial frame and generate the unwrap transformation
  cap >> frame;
  unwrapper.originalSize(frame.cols, frame.rows);
  unwrapper.generateTransformation();

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
  int run = 1;
  while(run)
  {
    //Start FPS timer for capture properties
    if(params.showCaptureProps)
      fpsTimer.start();

    //Capture a frame
    cap >> frame;

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
    if(params.mode[MODE_MJPG])
      imwrite(params.outputFilename[MODE_MJPG], unwrap);

    char keyPress = cv::waitKey(10);
    switch(keyPress)
    {
      //Exit on 'q' or ESC
      case 'q':
      case 27:
        printf("Exiting.\n");
        run = 0;
        break;
      case ' ':
        if(params.mode[MODE_STILLS])
        {
          //FOrmat filename with frame number
          int filenameLen = strlen(params.outputFilename[MODE_STILLS].c_str()) + 2;
          char stillFilename[filenameLen];
          sprintf(stillFilename, params.outputFilename[MODE_STILLS].c_str(), stillFrameNumber);
          printf("Saving still image: %s\n", stillFilename);
          //Save still image
          imwrite(stillFilename, unwrap);
          stillFrameNumber++;
        }
        break;
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
  }

  return 0;
}
