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
  SOURCE_V4L2     = 0,
  SOURCE_VIDEO,
  SOURCE_STILL,
  SOURCE_TIMELAPSE
};

/**
 * \brief Stores user options defining capture properties.
 */
struct BubbleScopeCaptureParams
{
  std::map<BubbleScopeCaptureMode, bool> mode;
  std::map<BubbleScopeCaptureMode, std::string> outputFilename;

  BubbleScopeCaptureSource captureSource;
  std::string captureLocation;

  unsigned int originalWidth;
  unsigned int originalHeight;

  bool unwrapCapture;

  double fps;
  unsigned int sampleFPS;
  double forceFPS;

  void print(std::ostream& stream);

  BubbleScopeCaptureParams();
};

/**
 * \brief Stores user options defining unwrap parameters.
 */
struct BubbleScopeUnwrapParams
{
  unsigned int unwrapWidth;
  float radiusMin;
  float radiusMax;
  float uCentre;
  float vCentre;
  float offsetAngle;

  template<class Archive>
  void serialize(Archive & ar,
      const unsigned int version);

  void print(std::ostream& stream);
  void load(std::string filename);
  void save(std::string filename);

  BubbleScopeUnwrapParams();
};

#endif
