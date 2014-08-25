#include "CommandLineParser.h"

namespace po = boost::program_options;

CommandLineParser::CommandLineParser() :
  m_captureParams(NULL), m_unwrapParams(NULL),
  m_helpWanted(false),
  m_generalOptions("General options"), m_captureOptions("Capture options"),
  m_unwrapOptions("Unwrap options"), m_outputOptions("Output options")
{
  m_generalOptions.add_options()
    ("help,h",        "Show usage and help")
    ("show-original", "Show the original capture in a window")
    ("show-unwrap",   "Show the unwrapped image in a window")
    ("config",        po::value<std::string>(), "Load unwrap parameters from config file")
    ("output-config", po::value<std::string>(), "Overwrite the config file with new parameters")
    ;

  m_captureOptions.add_options()
    ("v4l2-capture",      po::value<std::string>(), "Use V4L2 capture mode and set device to capture from")
    ("still-convert",     po::value<std::string>(), "Use still capture mode and specify image file")
    ("video-convert",     po::value<std::string>(), "Use video conversion mode and specify video file")
    ("timelapse-convert", po::value<std::string>(), "Use timelapse conversion mode and specify timelaspe frames")
    ("capture-width",     po::value<int>(),         "Sets the desired capture with for V4L2 sources")
    ("capture-height",    po::value<int>(),         "Sets the desired capture height fro V4L2 sources")
    ("sample-fps",        po::value<int>(),         "Sample V4L2 frame rate over a given number of samples")
    ("force-fps",         po::value<float>(),       "Force V4L2 frame rate to a given value")
    ;

  m_unwrapOptions.add_options()
    ("unwrap-width",  po::value<int>(),     "Sets the width of the unwrapped image (height is fixed based on this value)")
    ("min-radius",    po::value<double>(),  "Sets lower unwrap radius (0 - 0.5)")
    ("max-radius",    po::value<double>(),  "Sets the upper unwrap radius (0 - 0.5)")
    ("u-centre",      po::value<double>(),  "Sets U centre of original image (0 - 1)")
    ("v-centre",      po::value<double>(),  "Sets the V centre of original image (0 - 1)")
    ("offset-angle",  po::value<double>(),  "Sets unwrapped image offset angle")
    ("no-unwrap",     "Keeps image in original \"wrapped\" format")
    ;

  m_outputOptions.add_options()
    ("output-video",      po::value<std::string>(), "Specify location for captured/converted video")
    ("output-stills",     po::value<std::string>(), "Specify location for captured/converted images")
    ("output-mjpg",       po::value<std::string>(), "Specify location for MJPEG streamer frames")
    ("output-timelapse",  po::value<std::string>(), "")
    ("single-still",      "Capture a single still then exit")
    ;
}

CommandLineParser::~CommandLineParser()
{
  if(m_captureParams)
    delete m_captureParams;

  if(m_unwrapParams)
    delete m_unwrapParams;
}

BubbleScopeCaptureParams CommandLineParser::getCaptureParams()
{
  if(!m_captureParams)
    throw std::runtime_error("No options have been parsed");

  return *m_captureParams;
}

BubbleScopeUnwrapParams CommandLineParser::getUnwrapParams()
{
  if(!m_unwrapParams)
    throw std::runtime_error("No options have been parsed");

  return *m_unwrapParams;
}

bool CommandLineParser::helpWanted()
{
  return m_helpWanted;
}

void CommandLineParser::parse(int argc, char **argv)
{
  po::options_description all;
  all.add(m_generalOptions).add(m_captureOptions).add(m_unwrapOptions).add(m_outputOptions);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(all).run(), vm);
  po::notify(vm);

  m_helpWanted = vm.count("help");

  if(m_captureParams)
    delete m_captureParams;

  if(m_unwrapParams)
    delete m_unwrapParams;

  m_captureParams = new BubbleScopeCaptureParams();
  m_unwrapParams = new BubbleScopeUnwrapParams();

  //GENERAL OPTIONS
  if(vm.count("show-original"))
  {
    m_captureParams->mode[MODE_SHOW_ORIGINAL] = true;
  }

  if(vm.count("show-unwrap"))
  {
    m_captureParams->mode[MODE_SHOW_UNWRAP] = true;
  }

  //CAPTURE OPTIONS
  if(vm.count("v4l2-capture"))
  {
    m_captureParams->captureSource = SOURCE_V4L2;
    m_captureParams->captureLocation = vm["v4l2-capture"].as<std::string>();
  }
   
  if(vm.count("still-convert"))
  {
    m_captureParams->captureSource = SOURCE_STILL;
    m_captureParams->captureLocation = vm["still-convert"].as<std::string>();
  }
   
  if(vm.count("video-convert"))
  {
    m_captureParams->captureSource = SOURCE_VIDEO;
    m_captureParams->captureLocation = vm["video-convert"].as<std::string>();
  }
   
  if(vm.count("timelapse-convert"))
  {
    m_captureParams->captureSource = SOURCE_TIMELAPSE;
    m_captureParams->captureLocation = vm["timelapse-convert"].as<std::string>();
  }
   
  if(vm.count("capture-width"))
  {
    m_captureParams->originalWidth = vm["capture-width"].as<int>();
  }
   
  if(vm.count("capture-height"))
  {
    m_captureParams->originalWidth = vm["capture-height"].as<int>();
  }
   
  if(vm.count("sample-fps"))
  {
    m_captureParams->sampleFPS = vm["sample-fps"].as<int>();
  }
   
  if(vm.count("force-fps"))
  {
    m_captureParams->forceFPS = vm["force-fps"].as<float>();
  }

  //UNWRAP OPTIONS
  
  // Load config file before parsing other options
  if(vm.count("config"))
  {
    std::string filename = vm["config"].as<std::string>();
    m_unwrapParams->load(filename);
  }

  if(vm.count("unwrap-width"))
  {
    m_unwrapParams->unwrapWidth = vm["unwrap-width"].as<int>();
  }
   
  if(vm.count("min-radius"))
  {
    m_unwrapParams->radiusMin = vm["min-radius"].as<double>();
  }
   
  if(vm.count("max-radius"))
  {
    m_unwrapParams->radiusMax = vm["max-radius"].as<double>();
  }
   
  if(vm.count("u-centre"))
  {
    m_unwrapParams->uCentre = vm["u-centre"].as<double>();
  }
   
  if(vm.count("v-centre"))
  {
    m_unwrapParams->vCentre = vm["v-centre"].as<double>();
  }
   
  if(vm.count("offset-angle"))
  {
    m_unwrapParams->offsetAngle = vm["offset-angle"].as<double>();
  }

  //Save config file after parsing unwrap options
  if(vm.count("output-config"))
  {
    std::string filename = vm["output-config"].as<std::string>();
    m_unwrapParams->save(filename);
  }
   
  if(vm.count("no-unwrap"))
  {
    m_captureParams->unwrapCapture = false;
  }
 
  //OUTPUT OPTIONS  
  if(vm.count("output-video"))
  {
    m_captureParams->mode[MODE_VIDEO] = true;
    m_captureParams->outputFilename[MODE_VIDEO] = vm["output-video"].as<std::string>();
  }
   
  if(vm.count("output-stills"))
  {
    m_captureParams->mode[MODE_SHOW_UNWRAP] = true;
    m_captureParams->mode[MODE_STILLS] = true;
    m_captureParams->outputFilename[MODE_STILLS] = vm["output-stills"].as<std::string>();
  }
   
  if(vm.count("output-mjpg"))
  {
    m_captureParams->mode[MODE_MJPG] = true;
    m_captureParams->outputFilename[MODE_MJPG] = vm["output-mjpg"].as<std::string>();
  }
   
  if(vm.count("output-timelapse"))
  {
    m_captureParams->mode[MODE_TIMELAPSE] = true;
    m_captureParams->outputFilename[MODE_TIMELAPSE] = vm["output-timelapse"].as<std::string>();
  }
   
  if(vm.count("single-still"))
  {
    //Other capture modes will not work correctly in this mode
    m_captureParams->mode[MODE_SHOW_ORIGINAL] = false;
    m_captureParams->mode[MODE_SHOW_UNWRAP] = false;
    m_captureParams->mode[MODE_VIDEO] = false;
    m_captureParams->mode[MODE_STILLS] = false;
    m_captureParams->mode[MODE_SINGLE_STILL] = true;
    //Recycle some MJPG code to output image
    m_captureParams->outputFilename[MODE_MJPG] = vm["single-still"].as<std::string>();
  }
}

void CommandLineParser::printUsage(std::ostream &stream)
{
  stream << "Usage" << std::endl;
  stream << m_generalOptions << std::endl;
  stream << m_captureOptions << std::endl;
  stream << m_unwrapOptions << std::endl;
  stream << m_outputOptions << std::endl;
}
