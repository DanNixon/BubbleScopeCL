/*
 * BubbleScopeCL capture parameter functions.
 *
 * Dan Nixon
 */

#include "bubblescope_capture_params.h"

/*
 * Sets a resonable default configuration.
 */
void setupDefaultParameters(BubbleScopeParameters *params)
{
  params->captureSource = SOURCE_V4L2;
  params->captureLocation = "/dev/video0";
  params->originalWidth = 640;
  params->originalHeight = 480;
  params->unwrapCapture = true;
  params->unwrapWidth = 800;
  params->radiusMin = 0.25f;
  params->radiusMax = 0.45f;
  params->uCentre = 0.5f;
  params->vCentre = 0.5f;
  params->offsetAngle = 180.0f;
  params->mode[MODE_SHOW_ORIGINAL] = false;
  params->mode[MODE_SHOW_UNWRAP] = false;
  params->mode[MODE_SINGLE_STILL] = false;
  params->mode[MODE_STILLS] = false;
  params->mode[MODE_VIDEO] = false;
  params->mode[MODE_MJPG] = false;
  params->mode[MODE_TIMELAPSE] = 0;
  params->outputFilename[MODE_STILLS] = "BubbleScope_Still_Capture_%d.jpg";
  params->outputFilename[MODE_VIDEO] = "BubbleScope_Video_Capture.mkv";
  params->outputFilename[MODE_MJPG] = "BubbleScope_MJPG_Frame.jpg";
  params->outputFilename[MODE_TIMELAPSE] = "BubbleScope_Timelapse_Capture_%d.jpg";
  params->fps = 10.0f;
  params->sampleFPS = 0;
  params->forceFPS = -1.0;
  params->configFilename[CONFIG_READ] = "NONE";  //TODO: There must be a better way to do this
  params->configFilename[CONFIG_WRITE] = "NONE";
}

/*
 * Prints the current configuration to stdout.
 */
void printParameters(BubbleScopeParameters *params)
{
  printf("Source: %d, Location: %s\n", params->captureSource, params->captureLocation.c_str());
  printf("Framerate: %ffps (force %f)\n", params->fps, params->forceFPS);
  printf("Original image size: %dx%d\n", params->originalWidth, params->originalHeight);
  printf("Unwrap capture: %d\n", params->unwrapCapture);
  printf("Unwrap image width: %d\n", params->unwrapWidth);
  printf("Unwrap image radius: min=%f, max=%f\n", params->radiusMin, params->radiusMax);
  printf("Orignal image centre: u=%f, v=%f\n", params->uCentre, params->vCentre);
  printf("Offset angle: %fdeg.\n", params->offsetAngle);
  printf("Show original: %d\nShow unwrap: %d\n", params->mode[MODE_SHOW_ORIGINAL], params->mode[MODE_SHOW_UNWRAP]);
  printf("Stills output: %d, filename: %s\n", params->mode[MODE_STILLS], params->outputFilename[MODE_STILLS].c_str());
  printf("Video output: %d, FPS samples: %d, filename: %s\n", params->mode[MODE_VIDEO], params->sampleFPS,  params->outputFilename[MODE_VIDEO].c_str());
  printf("MJPG output: %d, Single still mode: %d, filename: %s\n", params->mode[MODE_MJPG], params->mode[MODE_SINGLE_STILL],
      params->outputFilename[MODE_MJPG].c_str());
  printf("Timelapse output delay: %dms, filename: %s\n", params->mode[MODE_TIMELAPSE], params->outputFilename[MODE_TIMELAPSE].c_str());
  printf("Read config: %s, write config: %s\n", params->configFilename[CONFIG_READ].c_str(), params->configFilename[CONFIG_WRITE].c_str());
}

/*
 * Reads the config file defined in configFilename[CONFIG_READ] and
 * overwrites all parameters in params which are defined in config.
 */
bool readConfigFromFile(BubbleScopeParameters *params)
{
  std::ifstream configFile(params->configFilename[CONFIG_READ].c_str());
  int lineNo = 0;

  std::string line;
  while (std::getline(configFile, line))
  {
    lineNo++;
    std::string::size_type pos = line.find_first_of(' ');
    if(pos != std::string::npos)
    {
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos+1, line.length());

      if(key == "source_v4l2")
      {
        params->captureSource = SOURCE_V4L2;
        params->captureLocation = value;
      }
      else if(key == "source_still")
      {
        params->captureSource = SOURCE_STILL;
        params->captureLocation = value;
      }
      else if(key == "source_video")
      {
        params->captureSource = SOURCE_VIDEO;
        params->captureLocation = value;
      }
      else if(key == "source_timelapse")
      {
        params->captureSource = SOURCE_TIMELAPSE;
        params->captureLocation = value;
      }
      else if(key == "original_width")
      {
        sscanf(value.c_str(), "%u", &(params->originalWidth));
      }
      else if(key == "original_height")
      {
        sscanf(value.c_str(), "%u", &(params->originalHeight));
      }
      else if(key == "unwrap_width")
      {
        sscanf(value.c_str(), "%u", &(params->unwrapWidth));
      }
      else if(key == "radius_min")
      {
        sscanf(value.c_str(), "%f", &(params->radiusMin));
      }
      else if(key == "radius_max")
      {
        sscanf(value.c_str(), "%f", &(params->radiusMax));
      }
      else if(key == "centre_u")
      {
        sscanf(value.c_str(), "%f", &(params->uCentre));
      }
      else if(key == "centre_v")
      {
        sscanf(value.c_str(), "%f", &(params->vCentre));
      }
      else if(key == "no_unwrap")
      {
        sscanf(value.c_str(), "%d", &(params->unwrapCapture));
      }
      else if(key == "show_original")
      {
        sscanf(value.c_str(), "%d", &(params->mode[MODE_SHOW_ORIGINAL]));
      }
      else if(key == "show_unwrap")
      {
        sscanf(value.c_str(), "%d", &(params->mode[MODE_SHOW_UNWRAP]));
      }
      else if(key == "force_fps")
      {
        sscanf(value.c_str(), "%f", &(params->forceFPS));
      }
      else if(key == "sample_fps")
      {
        sscanf(value.c_str(), "%d", &(params->sampleFPS));
      }
      else if(key == "output_stills")
      {
        params->mode[MODE_STILLS] = true;
        params->outputFilename[MODE_STILLS] = value;
      }
      else if(key == "output_video")
      {
        params->mode[MODE_VIDEO] = true;
        params->outputFilename[MODE_VIDEO] = value;
      }
      else if(key == "output_timelapse")
      {
        params->outputFilename[MODE_TIMELAPSE] = value;
      }
      else if(key == "output_timelapse_delay")
      {
        sscanf(value.c_str(), "%d", &(params->mode[MODE_TIMELAPSE]));
      }
      else if(key == "output_mjpg")
      {
        params->mode[MODE_MJPG] = true;
        params->outputFilename[MODE_MJPG] = value;
      }
      else if(key == "output_single_still")
      {
        params->mode[MODE_SINGLE_STILL] = true;
        params->outputFilename[MODE_MJPG] = value;
      }
    }
    else
    {
      printf("Config file parse error on line %d: %s\n", lineNo, line.c_str());
    }
  }

  return true;
}

/*
 * Writes the config to the config file defined in configFilename[CONFIG_WRITE]
 */
bool writeConfigToFile(BubbleScopeParameters *params)
{
  //TODO
  printf("WRITE CONFIG: %s\n", params->configFilename[CONFIG_WRITE].c_str());

  return false;
}
