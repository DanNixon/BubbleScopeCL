/*
 * Still image frame grabber
 * Grabs still image frame and image information from file
 *
 * Dan Nixon
 */

#include "source_imagefile.h"

void ImageFileSource::open(std::string imageFile)
{
  this->i_open = 1;
  this->image = cv::imread(imageFile);
}

void ImageFileSource::close()
{
}

int ImageFileSource::isOpen()
{
  return this->i_open;
}

void ImageFileSource::grab(cv::Mat *out)
{
  *out = this->image;
}

int ImageFileSource::getWidth()
{
  return this->image.cols;
}

int ImageFileSource::getHeight()
{
  return this->image.rows;
}
