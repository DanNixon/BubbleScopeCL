/*
 * Command line argument parser for BubbleScopeCL parameters.
 *
 * Dan Nixon
 */

#include "command_line_params.h"

/*
 * Array of parameters which may be passed to BubbleScope app
 */
CLParameter clParams[] = {
  CLParameter{HELP,             "-h",     "--help",           "Show help",          "Shows this help text"},
  CLParameter{CAPTURE_DEVICE,   "-d",     "--device",         "Capture device",     "Specifies a V4L2 capture device"},
  CLParameter{P_SOURCE_STILL,   "-ss",    "--sourcestill",    "Source still",       "Specifies a pre captured still to unwrap"},
  CLParameter{P_SOURCE_VIDEO,   "-sv",    "--sourcevideo",    "Source video",       "Specifies a pre recorded video to unwrap"},
  CLParameter{P_SOURCE_TLAPSE,  "-stl",   "--sourcetimelapse","Source timelapse",  "Specifies a set of pre captured timelapse frames to unwrap"},
  CLParameter{ORIGINAL_WIDTH,   "-iw",    "--inwidth",        "Original width",     "Set desired capture width (V4L2 only)"},
  CLParameter{ORIGINAL_HEIGHT,  "-ih",    "--inheight",       "Original height",    "Set desired capture height (V4L2 only)"},
  CLParameter{UNWRAP_WIDTH,     "-ow",    "--outwidth",       "Unwrap width",       "Set width of unwrapped image"},
  CLParameter{RADIUS_MIN,       "-rmin",  "--minradius",      "Radius min",         "Set lower unwrap radius"},
  CLParameter{RADIUS_MAX,       "-rmax",  "--maxradius",      "Radius max",         "Set upper unwrap radius"},
  CLParameter{U_CENTRE,         "-uc",    "--ucentre",        "U centre",           "Set U original image centre"},
  CLParameter{V_CENTRE,         "-vc",    "--vcentre",        "V centre",           "Set V original image centre"},
  CLParameter{OFFSET_ANGLE,     "-a",     "--offset",         "Offset angle",       "Set unwrap image offset angle"},
  CLParameter{SHOW_ORIGINAL,    "-o",     "--original",       "Show original",      "Show original video"},
  CLParameter{SHOW_UNWRAP,      "-u",     "--unwrap",         "Show unwrap",        "Show unwrapped video"},
  CLParameter{OUTPUT_STILLS,    "-s",     "--stills",         "Output stills",      "Capture stills on spacebar press"},
  CLParameter{OUTPUT_VIDEO,     "-v",     "--video",          "Output video",       "Capture AVI video"},
  CLParameter{OUTPUT_MJPG,      "-m",     "--mjpg",           "Output MJPG stream", "Output frames for MJPG streamer"},
  CLParameter{OUTPUT_TIMELAPSE, "-t",     "--timelapse",      "Output timelapse",   "Output captured frames as timelapse at specified interval"},
  CLParameter{SINGLE_STILL,     "-sin"    "--single",         "Capture 1 still",    "Capture a single still image and exit"},
  CLParameter{SAMPLE_FPS,       "-sfr",   "--samplefps",      "Sample frame rate",  "Specifies how many samples to use in measuring capture frame rate"},
  CLParameter{NO_UNWRAP,        "-nuw",   "--nounwrap",       "Do not unwrap image","Does not unwrap the captured image"}
};

/*
 * Size of params array
 */
int clParamCount = 21;

/*
 * Populates a set of BubbleScopeParameters based on contents of argv
 */
int getParameters(BubbleScopeParameters *params, int argc, char **argv)
{
  int i, j;
  for(i = 0; i < argc; i++)
  {
    for(j = 0; j < clParamCount; j++)
    {
      if((strcmp(clParams[j].shortParam, argv[i]) == 0) || (strcmp(clParams[j].longParam, argv[i]) == 0))
      {
        i++;
        char buffer[100];
        switch(clParams[j].type)
        {
          case HELP:
            return HELP;
          case CAPTURE_DEVICE:
            params->captureSource = SOURCE_V4L2;
            sscanf(argv[i], "%s", &buffer);
            params->captureLocation = buffer;
            break;
          case P_SOURCE_STILL:
            params->captureSource = SOURCE_STILL;
            sscanf(argv[i], "%s", &buffer);
            params->captureLocation = buffer;
            //Required to allow capture mode to function correctly
            params->mode[MODE_SHOW_ORIGINAL] = 0;
            params->mode[MODE_SHOW_UNWRAP] = 0;
            params->mode[MODE_MJPG] = 0;
            params->mode[MODE_VIDEO] = 0;
            break;
          case P_SOURCE_VIDEO:
            params->captureSource = SOURCE_VIDEO;
            sscanf(argv[i], "%s", &buffer);
            params->captureLocation = buffer;
            //Required to allow capture mode to function correctly
            params->mode[MODE_SHOW_ORIGINAL] = 0;
            params->mode[MODE_SHOW_UNWRAP] = 0;
            params->mode[MODE_MJPG] = 0;
            params->mode[MODE_STILLS] = 0;
            break;
          case P_SOURCE_TLAPSE:
            params->captureSource = SOURCE_TIMELAPSE;
            sscanf(argv[i], "%s", &buffer);
            params->captureLocation = buffer;
            //Required to allow capture mode to function correctly
            params->mode[MODE_SHOW_ORIGINAL] = 0;
            params->mode[MODE_SHOW_UNWRAP] = 0;
            params->mode[MODE_MJPG] = 0;
            params->mode[MODE_STILLS] = 0;
            break;
          case ORIGINAL_WIDTH:
            sscanf(argv[i], "%d", &params->originalWidth);
            break;
          case ORIGINAL_HEIGHT:
            sscanf(argv[i], "%d", &params->originalHeight);
            break;
          case UNWRAP_WIDTH:
            sscanf(argv[i], "%d", &params->unwrapWidth);
            break;
          case RADIUS_MIN:
            sscanf(argv[i], "%f", &params->radiusMin);
            break;
          case RADIUS_MAX:
            sscanf(argv[i], "%f", &params->radiusMax);
            break;
          case U_CENTRE:
            sscanf(argv[i], "%f", &params->uCentre);
            break;
          case V_CENTRE:
            sscanf(argv[i], "%f", &params->vCentre);
            break;
          case OFFSET_ANGLE:
            sscanf(argv[i], "%f", &params->offsetAngle);
            break;
          case SHOW_ORIGINAL:
            i--;
            params->mode[MODE_SHOW_ORIGINAL] = true;
            break;
          case SHOW_UNWRAP:
            i--;
            params->mode[MODE_SHOW_UNWRAP] = true;
            break;
          case OUTPUT_STILLS:
            params->mode[MODE_SHOW_UNWRAP] = true;
            params->mode[MODE_STILLS] = true;
            params->outputFilename[MODE_STILLS] = argv[i];
            break;
          case OUTPUT_VIDEO:
            params->mode[MODE_VIDEO] = true;
            params->outputFilename[MODE_VIDEO] = argv[i];
            break;
          case OUTPUT_MJPG:
            params->mode[MODE_MJPG] = true;
            params->outputFilename[MODE_MJPG] = argv[i];
            break;
          case OUTPUT_TIMELAPSE:
            sscanf(argv[i], "%d", &params->mode[MODE_TIMELAPSE]);
            params->outputFilename[MODE_TIMELAPSE] = argv[i + 1];
            i++;
            break;
          case SINGLE_STILL:
            //Other capture modes will not work correctly in this mode
            params->mode[MODE_SHOW_ORIGINAL] = false;
            params->mode[MODE_SHOW_UNWRAP] = false;
            params->mode[MODE_VIDEO] = false;
            params->mode[MODE_STILLS] = false;
            params->mode[MODE_SINGLE_STILL] = true;
            //Recycle some MJPG code to output image
            params->outputFilename[MODE_MJPG] = argv[i];
            break;
          case SAMPLE_FPS:
            sscanf(argv[i], "%d", &params->sampleFPS);
            break;
          case NO_UNWRAP:
            params->unwrapCapture = false;
            params->mode[MODE_SHOW_UNWRAP] = false;
            i--;
            break;
        }
      }
    }
  }
  return 0;
}

/*
 * Prints the parameters and their usage to stdout
 */
void printParameterUsage()
{
  int i;
  for(i = 0; i < clParamCount; i++)
  {
    printf(" %-5s\t%-12s\t: %-25s%s\n",
        clParams[i].shortParam, clParams[i].longParam,
        clParams[i].name, clParams[i].description);
  }
}
