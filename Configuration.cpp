#include "Configuration.h"

void BubbleScopeCaptureParams::print(std::ostream &stream)
{
  /* stream << ": " <<  << std::endl; */

  stream << "Capture Source: "    << captureSource    << std::endl;
  stream << "Capture Location: "  << captureLocation  << std::endl;
  stream << "Original Width: "    << originalWidth    << std::endl;
  stream << "Original Height: "   << originalHeight   << std::endl;
  stream << "Unwrap Capture: "    << unwrapCapture    << std::endl;
  stream << "Sample FPS: "        << sampleFPS        << std::endl;
  stream << "Force FPS: "         << forceFPS         << std::endl;
}

void BubbleScopeUnwrapParams::print(std::ostream &stream)
{
  stream << "Unwrap Width: "  << unwrapWidth  << std::endl;
  stream << "Radius Min: "    << radiusMin    << std::endl;
  stream << "Radius Max "     << radiusMax    << std::endl;
  stream << "U Centre: "      << uCentre      << std::endl;
  stream << "V Centre: "      << vCentre      << std::endl;
  stream << "Offset Angle: "  << offsetAngle  << std::endl;
}

void BubbleScopeUnwrapParams::load(std::string filename)
{
  //TODO
}

void BubbleScopeUnwrapParams::save(std::string filename)
{
  //TODO
}
