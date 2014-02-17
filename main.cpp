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
    printf("Can't open video capture source\n");
    return 2;
  }

  //The container for captured frames
  cv::Mat frame;

  //Capture an initial frame and generate the unwrap transformation
  cap >> frame;
  unwrapper.originalSize(frame.cols, frame.rows);
  unwrapper.generateTransformation();

  while(1)
  {
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

    //TODO: Add video saving, MJPG saving and stills capture on keypress

    //Exit if asked to  TODO: Should only do this if showing images in windows
    if(cv::waitKey(1) == 27)
      break;
  }
  return 0;
}
