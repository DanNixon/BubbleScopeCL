/*
 * BubbleScopeApp capture parameter functions.
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
  params->outputFilename[MODE_STILLS] = "BubbleScope_Still_Capture_%d.jpg";
  params->outputFilename[MODE_VIDEO] = "BubbleScope_Video_Capture.avi";
  params->outputFilename[MODE_MJPG] = "BubbleScope_MJPG_Frame.jpg";
  params->fps = 10.0f;
  params->sampleFPS = 0;
}

/*
 * Prints the current configuration to stdout.
 */
void printParameters(BubbleScopeParameters *params)
{
  printf("Source: %d, Location: %s\n", params->captureSource, params->captureLocation.c_str());
  printf("Framerate: %ffps\n", params->fps);
  printf("Original image size: %dx%d\n", params->originalWidth, params->originalHeight);
  printf("Unwrap image width: %d\n", params->unwrapWidth);
  printf("Unwrap image radius: min=%f, max=%f\n", params->radiusMin, params->radiusMax);
  printf("Orignal image centre: u=%f, v=%f\n", params->uCentre, params->vCentre);
  printf("Offset angle: %fdeg.\n", params->offsetAngle);
  printf("Show original: %d\nShow unwrap: %d\n", params->mode[MODE_SHOW_ORIGINAL], params->mode[MODE_SHOW_UNWRAP]);
  printf("Stills output: %d, filename: %s\n", params->mode[MODE_STILLS], params->outputFilename[MODE_STILLS].c_str());
  printf("Video output: %d, FPS samples: %d, filename: %s\n", params->mode[MODE_VIDEO], params->sampleFPS,  params->outputFilename[MODE_VIDEO].c_str());
  printf("MJPG output: %d, Single still mode: %d, filename: %s\n", params->mode[MODE_MJPG], params->mode[MODE_SINGLE_STILL],
      params->outputFilename[MODE_MJPG].c_str());
}
