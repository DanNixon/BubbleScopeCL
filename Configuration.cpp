#include "Configuration.h"

#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

BubbleScopeCaptureParams::BubbleScopeCaptureParams() :
  captureSource(SOURCE_V4L2), captureLocation("/dev/video0"),
  originalWidth(640), originalHeight(480),
  unwrapCapture(true), sampleFPS(0), forceFPS(-1.0)
{
}

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

BubbleScopeUnwrapParams::BubbleScopeUnwrapParams() :
  unwrapWidth(1000),
  radiusMin(0.2), radiusMax(0.4),
  uCentre(0.5), vCentre(0.5),
  offsetAngle(180.0)
{
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

template<class Archive>
void BubbleScopeUnwrapParams::serialize(Archive & ar,
    const unsigned int version)
{
  ar & unwrapWidth;
  ar & radiusMin;
  ar & radiusMax;
  ar & uCentre;
  ar & vCentre;
  ar & offsetAngle;
}


void BubbleScopeUnwrapParams::load(std::string filename)
{
  std::ifstream inputStream(filename.c_str());
  boost::archive::text_iarchive inputArchive(inputStream);
  inputArchive >> *this;
}

void BubbleScopeUnwrapParams::save(std::string filename)
{
  std::ofstream outputStream(filename.c_str());
  boost::archive::text_oarchive outputArchive(outputStream);
  outputArchive << *this;
}
