/*
 * BubbleScopeCL capture parameter config file functions.
 *
 * Dan Nixon
 */

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "bubblescope_capture_params.h"

enum ConfigFileElements
{
  CFG_SOURCE_V4L2,
  CFG_SOURCE_STILL,
  CFG_SOURCE_VIDEO,
  CFG_SOURCE_TIMELAPSE,
  CFG_ORIGINAL_WIDTH,
  CFG_ORIGINAL_HEIGHT,
  CFG_UNWRAP_WIDTH,
  CFG_RADIUS_MIN,
  CFG_RADIUS_MAX,
  CFG_CENTRE_U,
  CFG_CENTRE_V,
  CFG_UNWRAP,
  CFG_SHOW_ORIGINAL,
  CFG_SHOW_UNWRAP,
  CFG_FORCE_FPS,
  CFG_SAMPLE_FPS,
  CFG_OUTPUT_STILLS,
  CFG_OUTPUT_VIDEO,
  CFG_OUTPUT_TIMELAPSE,
  CFG_OUTPUT_TIMELAPSE_DELAY,
  CFG_OUTPUT_MJPG,
  CFG_OUTPUT_SINGLE_STILL
};

bool readConfigFromFile(BubbleScopeParameters *);
bool writeConfigToFile(BubbleScopeParameters *);

#endif
