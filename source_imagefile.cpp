#include "source_imagefile.h"

void StillImageSource::open(std::string imageFile)
{
  this->i_open = 1;
  this->image = cv::imread(imageFile);
}

void StillImageSource::close()
{
}

int StillImageSource::isOpen()
{
  return this->i_open;
}

cv::Mat StillImageSource::grab()
{
  return this->image;
}

int StillImageSource::getWidth()
{
  return this->image.cols;
}

int StillImageSource::getHeight()
{
  return this->image.rows;
}
