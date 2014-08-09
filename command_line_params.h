/**
 * \brief Command line argument parser for BubbleScopeCL parameters.
 *
 * \author Dan Nixon
 */

#ifndef COMMANDLINEPARAMS_H
#define COMMANDLINEPARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config_file.h"

/**
 * \brief Sets the type of a parameter
 */
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

/**
 * \brief Stores details about a command line parameter
 */
struct CLParameter
{
  ParameterType m_type;
  char *m_shortParam;
  char *m_longParam;
  char *m_name;
  char *m_description;

  CLParameter(ParameterType type, char *shortParam, char *longParam,
      char *name, char *description) :
    m_type(type), m_shortParam(shortParam), m_longParam(longParam),
    m_name(name), m_description(description)
  {
  }
 };

int getParameters(BubbleScopeParameters *, int, char **);
void printParameterUsage();

#endif
