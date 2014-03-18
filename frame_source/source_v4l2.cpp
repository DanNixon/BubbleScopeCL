/*
 * V4L2 frame grabber
 * Grabs frames and image information from a V4L2 source
 *
 * Dan Nixon
 */

#include "source_v4l2.h"

V4L2Source::V4L2Source()
{
  this->o_capture = new OCVCapture();
  this->o_capture->setVerbose(false);
}

V4L2Source::~V4L2Source()
{
  this->close();
  delete this->o_capture;
}

void V4L2Source::open(std::string deviceName)
{
  this->o_capture->open(deviceName.c_str());
}

void V4L2Source::close()
{
  this->o_capture->close();
}

bool V4L2Source::isOpen()
{
  return this->o_capture->isOpen();
}

bool V4L2Source::grab(cv::Mat *out)
{
  this->o_capture->grab();
  this->o_capture->rgb(*out);
  return true;
}

unsigned int V4L2Source::getWidth()
{
  return this->o_capture->width();
}

unsigned int V4L2Source::getHeight()
{
  return this->o_capture->height();
}

void V4L2Source::setCaptureSize(unsigned int width, unsigned int height)
{
  this->o_capture->setDesiredSize(width, height);
}

double V4L2Source::getFrameRate()
{
  return this->o_capture->frameRate();
}
