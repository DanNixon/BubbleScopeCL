
#include "source_v4l2.h"

V4L2Source::V4L2Source()
{
  this->cap = new OCVCapture();
  this->cap->setVerbose(false);
}

V4L2Source::~V4L2Source()
{
  this->close();
  delete this->cap;
}

void V4L2Source::open(std::string deviceName)
{
  this->cap->open(deviceName.c_str());
}

void V4L2Source::close()
{
  this->cap->close();
}

int V4L2Source::isOpen()
{
  return this->cap->isOpen();
}

void V4L2Source::grab(cv::Mat *out)
{
  this->cap->grab();
  this->cap->rgb(*out);
}

int V4L2Source::getWidth()
{
  return this->cap->width();
}

int V4L2Source::getHeight()
{
  return this->cap->height();
}

void V4L2Source::setCaptureSize(int width, int height)
{
  this->cap->setDesiredSize(width, height);
}

int V4L2Source::getFrameRate()
{
  return this->cap->frameRate();
}
