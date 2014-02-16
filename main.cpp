#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "unwrap.h"

enum BubbleScopeCaptureMode
{
  PREVIEW,
  VIDEO,
  STILLS
};

struct BubbleScopeParameters
{
  int captureDevice;
  int originalWidth;
  int originalHeight;
  int unwrapWidth;
  float radiusMin;
  float radiusMax;
  float uCentre;
  float vCentre;
  float offsetAngle;
  int showOriginal;
  int showUnwrap;
  BubbleScopeCaptureMode capMode;
  std::string outputFilename;
};

void setupDefaultParameters(BubbleScopeParameters *params)
{
  params->captureDevice = 0;
  params->originalWidth = 0;
  params->originalHeight = 0;
  params->unwrapWidth = 800;
  params->radiusMin = 0.25f;
  params->radiusMax = 0.6f;
  params->uCentre = 0.5f;
  params->vCentre = 0.5f;
  params->offsetAngle = (180.0f * 0.0174532925f);
  params->showOriginal = 0;
  params->showUnwrap = 1;
  params->capMode = PREVIEW;
  params->outputFilename = "BubbleScope_Capture";
}

void printParameters(BubbleScopeParameters *params)
{
  printf("Video caputre device: %d\n", params->captureDevice);
  printf("Original image size: %dx%d\n", params->originalWidth, params->originalHeight);
  printf("Unwrap image width: %d\n", params->unwrapWidth);
  printf("Unwrap image radius: min=%f, max=%f\n", params->radiusMin, params->radiusMax);
  printf("Orignal image centre: u=%f, v=%f\n", params->uCentre, params->vCentre);
  printf("Offset angle: %f rad.\n", params->offsetAngle);
  printf("Show original: %d\nShow unwrap: %d\n", params->showOriginal, params->showUnwrap);
  std::string mode;
  switch(params->capMode)
  {
    case PREVIEW:
      mode = "Preview";
      break;
    case VIDEO:
      mode = "Video";
      break;
    case STILLS:
      mode = "Stills";
      break;
  }
  printf("Capture mode: %s\n", mode.c_str());
  printf("Output filename: %s\n", params->outputFilename.c_str());
}

int main(int argc, char **argv)
{
  BubbleScopeParameters params;
  setupDefaultParameters(&params);

  sscanf(argv[1], "%d", &params.captureDevice);
  sscanf(argv[2], "%d", &params.originalWidth);
  sscanf(argv[3], "%d", &params.originalHeight);
  sscanf(argv[4], "%d", &params.unwrapWidth);
  sscanf(argv[5], "%f", &params.radiusMin);
  sscanf(argv[6], "%f", &params.radiusMax);
  sscanf(argv[7], "%f", &params.uCentre);
  sscanf(argv[8], "%f", &params.vCentre);
  sscanf(argv[9], "%d", &params.showOriginal);

  printParameters(&params);

  BubbleScopeUnwrapper bsu;
  bsu.unwrapWidth(params.unwrapWidth);
  bsu.originalCentre(params.uCentre, params.vCentre);
  bsu.imageRadius(params.radiusMin, params.radiusMax);
  bsu.offsetAngle(params.offsetAngle);

  cv::VideoCapture cap(params.captureDevice);

  if(!cap.isOpened())
  {
    printf("Can't open capture\n");
    return -1;
  }

  int first = 1;

  while(1)
  {
    cv::Mat frame;
    cap >> frame;

    if(first)
    {
      bsu.originalSize(frame.cols, frame.rows);
      bsu.generateTransformation();
      first = 0;
    }

    cv::Mat out = bsu.unwrap(&frame);

    if(params.showOriginal)
      imshow("original", frame);

    if(params.showUnwrap)
      imshow("unwrap", out);

    if(cv::waitKey(10) == 27)
      break;
  }
  return 0;
}
