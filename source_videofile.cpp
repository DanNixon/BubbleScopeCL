
#include "source_videofile.h"

void VideoSource::open(std::string videoFile)
{
}

void VideoSource::close()
{
}

int VideoSource::isOpen()
{
  return 0;
}

cv::Mat VideoSource::grab()
{
  return NULL;
}

int VideoSource::getWidth()
{
  return 0;
}

int VideoSource::getHeight()
{
  return 0;
}

int VideoSource::getFrameRate()
{
  return 0;
}

int VideoSource::getImageType()
{
  return 0;
}
