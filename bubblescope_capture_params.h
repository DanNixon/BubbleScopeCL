/**
 * \brief BubbleScopeCL capture parameter functions.
 *
 * \author Dan Nixon
 */

#ifndef BUBBLESCOPECAPPARMAS_H
#define BUBBLESCOPECAPPARMAS_H

#include <stdio.h>
#include <string>

/**
 * \brief Defines capture modes
 */
enum BubbleScopeCaptureMode
{
  MODE_STILLS         = 0,
  MODE_VIDEO          = 1,
  MODE_MJPG           = 2,
  MODE_TIMELAPSE      = 3,
  MODE_SINGLE_STILL,
  MODE_SHOW_ORIGINAL,
  MODE_SHOW_UNWRAP
};

/**
 * \brief Defines selected caputre source
 */
enum BubbleScopeCaptureSource
{
  SOURCE_V4L2,
  SOURCE_VIDEO,
  SOURCE_STILL,
  SOURCE_TIMELAPSE
};

/**
 * \brief Config file directions
 */
enum BubbleScopeConfigFile
{
  CONFIG_READ,
  CONFIG_WRITE
};

/**
 * \brief Stores user options defining capture properties.
 */
struct BubbleScopeParameters
{
  BubbleScopeCaptureSource captureSource;
  std::string captureLocation;
  unsigned int originalWidth;
  unsigned int originalHeight;
  bool unwrapCapture;
  unsigned int unwrapWidth;
  float radiusMin;
  float radiusMax;
  float uCentre;
  float vCentre;
  float offsetAngle;
  unsigned int mode[7];
  std::string outputFilename[4];
  float fps;
  unsigned int sampleFPS;
  float forceFPS;
  std::string configFilename[2];
};

void setupDefaultParameters(BubbleScopeParameters *);
void printParameters(BubbleScopeParameters *);

#endif
