/*
 * Still image frame grabber
 * Grabs still image frame and image information from file
 *
 * Dan Nixon
 */

#include "source_imagefile.h"

void ImageFileSource::open(std::string imageFile)
{
  this->b_open = true;
  this->o_image = cv::imread(imageFile);
}

void ImageFileSource::close()
{
}

bool ImageFileSource::isOpen()
{
  return this->b_open;
}

bool ImageFileSource::grab(cv::Mat *out)
{
  *out = this->o_image;
  return true;
}

unsigned int ImageFileSource::getWidth()
{
  return this->o_image.cols;
}

unsigned int ImageFileSource::getHeight()
{
  return this->o_image.rows;
}
