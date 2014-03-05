
#include "source_v4l2.h"

V4L2Source::V4L2Source()
{
  this->cap.setVerbose(false);
}

void V4L2Source::open(std::string deviceName)
{
  printf("Opening %s\n", deviceName.c_str());
  this->cap.open(deviceName.c_str());
}

void V4L2Source::close()
{
  this->cap.close();
}

int V4L2Source::isOpen()
{
  return this->cap.isOpen();
}

cv::Mat V4L2Source::grab()
{
  this->cap.grab();
}

int V4L2Source::getWidth()
{
  return this->cap.width();
}

int V4L2Source::getHeight()
{
  return this->cap.height();
}

void V4L2Source::setCaptureSize(int width, int height)
{
  this->cap.setDesiredSize(width, height);
}

int V4L2Source::getFrameRate()
{
  return 0;
}
