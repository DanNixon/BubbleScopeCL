/*
 * Timelapse frame grabber
 * Grabs still image frames using filename pattern with incrementing number
 *
 * Dan Nixon
 */

#include "source_timelapse.h"

TimelapseSource::TimelapseSource()
{
  this->frameNumber = 0;
}

TimelapseSource::~TimelapseSource()
{
  delete this->filenamePattern;
}

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

bool TimelapseSource::grab(cv::Mat *out)
{
  //Generate filename
  unsigned int filenameLen = strlen(this->filenamePattern) + 5;
  char *imageFilename = new char[filenameLen];
  sprintf(imageFilename, this->filenamePattern, this->frameNumber);
  
  struct stat buffer;   
  if(stat (imageFilename, &buffer))
    return false;

  *out = cv::imread(imageFilename);

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
