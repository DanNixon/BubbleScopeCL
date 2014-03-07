/*
 * Still image frame grabber
 * Grabs still image frame and image information from file
 *
 * Dan Nixon
 */

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

void StillImageSource::grab(cv::Mat *out)
{
  *out = this->image;
}

int StillImageSource::getWidth()
{
  return this->image.cols;
}

int StillImageSource::getHeight()
{
  return this->image.rows;
}
