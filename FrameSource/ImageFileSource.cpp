#include "ImageFileSource.h"

/**
 * \brief Open a still image file from disk
 *
 * \param imageFile The image file to open
 */
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

/**
 * \brief Gets the contents of the still image
 *
 * \param out The matrix to copy image to
 *
 * \return If the capture was successfull
 */
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
