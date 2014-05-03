#include "config_file.h"

using namespace std;

string configStrings[] =
{
  "source_v4l2",
  "source_still",
  "source_video",
  "source_timelapse",
  "original_width",
  "original_height",
  "unwrap_width",
  "radius_min",
  "radius_max",
  "centre_u",
  "centre_v",
  "unwrap",
  "show_original",
  "show_unwrap",
  "force_fps",
  "sample_fps",
  "output_stills",
  "output_video",
  "output_timelapse",
  "output_timelapse_delay",
  "output_mjpg",
  "output_single_still"
};

/*
 * Reads the config file defined in configFilename[CONFIG_READ] and
 * overwrites all parameters in params which are defined in config.
 */
bool readConfigFromFile(BubbleScopeParameters *params)
{
  ifstream configFile(params->configFilename[CONFIG_READ].c_str());
  int lineNo = 0;

  string line;
  while (getline(configFile, line))
  {
    lineNo++;
    string::size_type pos = line.find_first_of(' ');
    if(pos != string::npos)
    {
      string key = line.substr(0, pos);
      string value = line.substr(pos+1, line.length());

      if(key == configStrings[CFG_SOURCE_V4L2])
      {
        params->captureSource = SOURCE_V4L2;
        params->captureLocation = value;
      }
      else if(key == configStrings[CFG_SOURCE_STILL])
      {
        params->captureSource = SOURCE_STILL;
        params->captureLocation = value;
      }
      else if(key == configStrings[CFG_SOURCE_VIDEO])
      {
        params->captureSource = SOURCE_VIDEO;
        params->captureLocation = value;
      }
      else if(key == configStrings[CFG_SOURCE_TIMELAPSE])
      {
        params->captureSource = SOURCE_TIMELAPSE;
        params->captureLocation = value;
      }
      else if(key == configStrings[CFG_ORIGINAL_WIDTH])
      {
        sscanf(value.c_str(), "%u", &(params->originalWidth));
      }
      else if(key == configStrings[CFG_ORIGINAL_HEIGHT])
      {
        sscanf(value.c_str(), "%u", &(params->originalHeight));
      }
      else if(key == configStrings[CFG_UNWRAP_WIDTH])
      {
        sscanf(value.c_str(), "%u", &(params->unwrapWidth));
      }
      else if(key == configStrings[CFG_RADIUS_MIN])
      {
        sscanf(value.c_str(), "%f", &(params->radiusMin));
      }
      else if(key == configStrings[CFG_RADIUS_MAX])
      {
        sscanf(value.c_str(), "%f", &(params->radiusMax));
      }
      else if(key == configStrings[CFG_CENTRE_U])
      {
        sscanf(value.c_str(), "%f", &(params->uCentre));
      }
      else if(key == configStrings[CFG_CENTRE_V])
      {
        sscanf(value.c_str(), "%f", &(params->vCentre));
      }
      else if(key == configStrings[CFG_UNWRAP])
      {
        sscanf(value.c_str(), "%d", &(params->unwrapCapture));
      }
      else if(key == configStrings[CFG_SHOW_ORIGINAL])
      {
        sscanf(value.c_str(), "%d", &(params->mode[MODE_SHOW_ORIGINAL]));
      }
      else if(key == configStrings[CFG_SHOW_UNWRAP])
      {
        sscanf(value.c_str(), "%d", &(params->mode[MODE_SHOW_UNWRAP]));
      }
      else if(key == configStrings[CFG_FORCE_FPS])
      {
        sscanf(value.c_str(), "%f", &(params->forceFPS));
      }
      else if(key == configStrings[CFG_SAMPLE_FPS])
      {
        sscanf(value.c_str(), "%d", &(params->sampleFPS));
      }
      else if(key == configStrings[CFG_OUTPUT_STILLS])
      {
        params->mode[MODE_STILLS] = true;
        params->outputFilename[MODE_STILLS] = value;
      }
      else if(key == configStrings[CFG_OUTPUT_VIDEO])
      {
        params->mode[MODE_VIDEO] = true;
        params->outputFilename[MODE_VIDEO] = value;
      }
      else if(key == configStrings[CFG_OUTPUT_TIMELAPSE])
      {
        params->outputFilename[MODE_TIMELAPSE] = value;
      }
      else if(key == configStrings[CFG_OUTPUT_TIMELAPSE_DELAY])
      {
        sscanf(value.c_str(), "%d", &(params->mode[MODE_TIMELAPSE]));
      }
      else if(key == configStrings[CFG_OUTPUT_MJPG])
      {
        params->mode[MODE_MJPG] = true;
        params->outputFilename[MODE_MJPG] = value;
      }
      else if(key == configStrings[CFG_OUTPUT_SINGLE_STILL])
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

  configFile.close();
  return true;
}

/*
 * Writes the config to the config file defined in configFilename[CONFIG_WRITE]
 */
bool writeConfigToFile(BubbleScopeParameters *params)
{
  ofstream configFile(params->configFilename[CONFIG_WRITE].c_str());

  switch(params->captureSource)
  {
    case SOURCE_V4L2:
      configFile << configStrings[CFG_SOURCE_V4L2] << " ";
      break;
    case SOURCE_VIDEO:
      configFile << configStrings[CFG_SOURCE_VIDEO] << " ";
      break;
    case SOURCE_STILL:
      configFile << configStrings[CFG_SOURCE_STILL] << " ";
      break;
    case SOURCE_TIMELAPSE:
      configFile << configStrings[CFG_SOURCE_TIMELAPSE] << " ";
      break;
  }
  configFile << params->captureLocation << endl;

  configFile << configStrings[CFG_ORIGINAL_WIDTH] <<
    " " << params->originalWidth << endl;
  configFile << configStrings[CFG_ORIGINAL_HEIGHT] <<
    " " << params->originalHeight << endl;
  configFile << configStrings[CFG_UNWRAP_WIDTH] <<
    " " << params->unwrapWidth << endl;

  configFile << configStrings[CFG_RADIUS_MIN] <<
    " " << params->radiusMin << endl;
  configFile << configStrings[CFG_RADIUS_MAX] <<
    " " << params->radiusMax << endl;
  configFile << configStrings[CFG_CENTRE_U] <<
    " " << params->uCentre << endl;
  configFile << configStrings[CFG_CENTRE_V] <<
    " " << params->vCentre << endl;

  configFile << configStrings[CFG_UNWRAP] <<
    " " << params->unwrapCapture << endl;
  configFile << configStrings[CFG_SHOW_ORIGINAL] <<
    " " << params->mode[MODE_SHOW_ORIGINAL] << endl;
  configFile << configStrings[CFG_SHOW_UNWRAP] <<
    " " << params->mode[MODE_SHOW_UNWRAP] << endl;

  configFile << configStrings[CFG_FORCE_FPS] <<
    " " << params->forceFPS << endl;
  configFile << configStrings[CFG_SAMPLE_FPS] <<
    " " << params->sampleFPS << endl;

  if(params->mode[MODE_STILLS])
  {
    configFile << configStrings[CFG_OUTPUT_STILLS] <<
      " " << params->outputFilename[MODE_STILLS] << endl;
  }

  if(params->mode[MODE_VIDEO])
  {
    configFile << configStrings[CFG_OUTPUT_VIDEO] <<
      " " << params->outputFilename[MODE_VIDEO] << endl;
  }

  if(params->mode[MODE_MJPG])
  {
    configFile << configStrings[CFG_OUTPUT_MJPG] <<
      " " << params->outputFilename[MODE_MJPG] << endl;
  }

  if(params->mode[MODE_SINGLE_STILL])
  {
    configFile << configStrings[CFG_OUTPUT_SINGLE_STILL] <<
      " " << params->outputFilename[MODE_MJPG] << endl;
  }

  if(params->mode[MODE_TIMELAPSE] > 0)
  {
    configFile << configStrings[CFG_OUTPUT_TIMELAPSE] <<
      " " << params->outputFilename[MODE_TIMELAPSE] << endl;
    configFile << configStrings[CFG_OUTPUT_TIMELAPSE_DELAY] <<
      " " << params->mode[MODE_TIMELAPSE] << endl;
  }

  configFile.close();
  return true;
}
