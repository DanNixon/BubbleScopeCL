#include "ImageFileSource.h"

ImageFileSource::ImageFileSource():
  m_open(false)
{
}

/**
 * \brief Open a still image file from disk
 *
 * \param imageFile The image file to open
 */
void ImageFileSource::open(std::string imageFile)
{
  m_open = true;
  m_image = cv::imread(imageFile);
}

void ImageFileSource::close()
{
}

bool ImageFileSource::isOpen()
{
  return m_open;
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
  *out = m_image;
  return true;
}

unsigned int ImageFileSource::getWidth()
{
  return m_image.cols;
}

unsigned int ImageFileSource::getHeight()
{
  return m_image.rows;
}
