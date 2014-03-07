/*
 * Video file frame grabber
 * Grabs frames and image information for a video file
 *
 * Dan Nixon
 */

#include "source_videofile.h"

VideoFileSource::VideoFileSource()
{
  this->cap = NULL;
  this->i_grabbedFrameCount = 0;
}

VideoFileSource::~VideoFileSource()
{
  delete this->cap;
}

void VideoFileSource::open(std::string videoFile)
{
  this->cap = new cv::VideoCapture(videoFile);
  this->cap->set(CV_CAP_PROP_CONVERT_RGB, 1);
  this->i_grabbedFrameCount = 0;
}

void VideoFileSource::close()
{
  this->cap->release();
}

int VideoFileSource::isOpen()
{
  return this->cap->isOpened();
}

void VideoFileSource::grab(cv::Mat *out)
{
  this->cap->read(*out);
  this->i_grabbedFrameCount++;
}

int VideoFileSource::getWidth()
{
  return this->cap->get(CV_CAP_PROP_FRAME_WIDTH);
}

int VideoFileSource::getHeight()
{
  return this->cap->get(CV_CAP_PROP_FRAME_HEIGHT);
}

int VideoFileSource::getFrameRate()
{
  return this->cap->get(CV_CAP_PROP_FPS);
}

unsigned int VideoFileSource::getGrabbedFrameCount()
{
  return this->i_grabbedFrameCount;
}

int VideoFileSource::getFrameCount()
{
  return this->cap->get(CV_CAP_PROP_FRAME_COUNT);
}

int VideoFileSource::atEndOfVideo()
{
  return (this->getGrabbedFrameCount() >= this->getFrameCount());
}
