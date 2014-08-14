#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <map>

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
 * \brief Stores user options defining capture properties.
 */
struct CaptureParams
{
  std::map<BubbleScopeCaptureMode, bool> mode;
  std::map<BubbleScopeCaptureMode, std::string> outputFilename;

  BubbleScopeCaptureSource captureSource;
  std::string captureLocation;

  unsigned int originalWidth;
  unsigned int originalHeight;

  bool unwrapCapture;

  float fps;
  unsigned int sampleFPS;
  float forceFPS;

  void print(std::ostream);
};

/**
 * \brief Stores user options defining unwrap parameters.
 */
struct UnwrapParams
{
  unsigned int unwrapWidth;
  float radiusMin;
  float radiusMax;
  float uCentre;
  float vCentre;
  float offsetAngle;

  void print(std::ostream);
  void load(std::string);
  void save(std::string);
};

#endif
