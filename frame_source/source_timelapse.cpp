#include "source_timelapse.h"

TimelapseSource::TimelapseSource()
{
  this->frameNumber = 0;
}

TimelapseSource::~TimelapseSource()
{
  delete this->filenamePattern;
}

/**
 * \brief Sets filename pattern for timelapse frames
 *
 * \param path The path with filename pattern
 */
void TimelapseSource::open(std::string path)
{
  this->filenamePattern = (char *) path.c_str();
  this->frameNumber = 0;
}

void TimelapseSource::close()
{
  this->filenamePattern = NULL;
  this->frameNumber = 0;
}

bool TimelapseSource::isOpen()
{
  return (this->filenamePattern != NULL);
}

/**
 * \brief Grabs the next frame mathing the naming pattern
 *
 * \param out The matrix to copy image to
 *
 * \return True on successful capture, false otherwise
 */
bool TimelapseSource::grab(cv::Mat *out)
{
  //Generate filename
  unsigned int filenameLen = strlen(this->filenamePattern) + 5;
  char *imageFilename = new char[filenameLen];
  sprintf(imageFilename, this->filenamePattern, this->frameNumber);
  
  struct stat buffer;   
  if(stat (imageFilename, &buffer))
    return false;

  this->o_frame = cv::imread(imageFilename);
  *out = this->o_frame;

  //Increment frame number
  this->frameNumber++;

  return true;
}

unsigned int TimelapseSource::getWidth()
{
  cv::Mat frame;
  this->grab(&frame);
  this->frameNumber--;
  return frame.cols;
}

unsigned int TimelapseSource::getHeight()
{
  cv::Mat frame;
  this->grab(&frame);
  this->frameNumber--;
  return frame.rows;
}
