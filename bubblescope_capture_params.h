#ifndef BUBBLESCOPECAPPARMAS_H
#define BUBBLESCOPECAPPARMAS_H

#include <stdio.h>
#include <string>

enum BubbleScopeCaptureMode
{
  MODE_STILLS         = 0,
  MODE_VIDEO          = 1,
  MODE_MJPG           = 2,
  MODE_SINGLE_STILL,
  MODE_SHOW_ORIGINAL,
  MODE_SHOW_UNWRAP,
};

enum BubbleScopeCaptureSource
{
  SOURCE_V4L2,
  SOURCE_VIDEO,
  SOURCE_STILL
};

/*
 * Stores user options defining capture properties.
 */
struct BubbleScopeParameters
{
  BubbleScopeCaptureSource captureSource;
  std::string captureLocation;
  unsigned int originalWidth;
  unsigned int originalHeight;
  unsigned int unwrapWidth;
  float radiusMin;
  float radiusMax;
  float uCentre;
  float vCentre;
  float offsetAngle;
  bool mode[6];
  std::string outputFilename[3];
  float fps;
  bool showCaptureProps;
  unsigned int sampleFPS;
};

void setupDefaultParameters(BubbleScopeParameters *);
void printParameters(BubbleScopeParameters *);

#endif
