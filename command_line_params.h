/*
 * Command line argument parser for BubbleScopeCL parameters.
 *
 * Dan Nixon
 */

#ifndef COMMANDLINEPARAMS_H
#define COMMANDLINEPARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bubblescope_capture_params.h"

enum ParameterType
{
  CAPTURE_DEVICE,
  P_SOURCE_STILL,
  P_SOURCE_VIDEO,
  P_SOURCE_TLAPSE,
  ORIGINAL_WIDTH,
  ORIGINAL_HEIGHT,
  UNWRAP_WIDTH,
  RADIUS_MIN,
  RADIUS_MAX,
  U_CENTRE,
  V_CENTRE,
  OFFSET_ANGLE,
  SHOW_ORIGINAL,
  SHOW_UNWRAP,
  OUTPUT_VIDEO,
  OUTPUT_STILLS,
  OUTPUT_MJPG,
  OUTPUT_TIMELAPSE,
  SINGLE_STILL,
  SAMPLE_FPS,
  FORCE_FPS,
  LOAD_CONFIG,
  SAVE_CONFIG,
  NO_UNWRAP,

  HELP = 255
};

struct CLParameter
{
  ParameterType type;
  char *shortParam;
  char *longParam;
  char *name;
  char *description;
 };

int getParameters(BubbleScopeParameters *, int, char **);
void printParameterUsage();

#endif
