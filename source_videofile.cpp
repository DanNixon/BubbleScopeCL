/*
 * Video file frame grabber
 * Grabs frames and image information for a video file
 *
 * Dan Nixon
 */

#include "source_videofile.h"

VideoFileSource::VideoFileSource()
{
  this->o_capture = NULL;
  this->i_grabbedFrameCount = 0;
}

VideoFileSource::~VideoFileSource()
{
  delete this->o_capture;
}

void VideoFileSource::open(std::string videoFile)
{
  this->o_capture = new cv::VideoCapture(videoFile);
  this->o_capture->set(CV_CAP_PROP_CONVERT_RGB, 1);
  this->i_grabbedFrameCount = 0;
}

void VideoFileSource::close()
{
  this->o_capture->release();
}

bool VideoFileSource::isOpen()
{
  return this->o_capture->isOpened();
}

void VideoFileSource::grab(cv::Mat *out)
{
  this->o_capture->read(*out);
  this->i_grabbedFrameCount++;
}

unsigned int VideoFileSource::getWidth()
{
  return this->o_capture->get(CV_CAP_PROP_FRAME_WIDTH);
}

unsigned int VideoFileSource::getHeight()
{
  return this->o_capture->get(CV_CAP_PROP_FRAME_HEIGHT);
}

double VideoFileSource::getFrameRate()
{
  return this->o_capture->get(CV_CAP_PROP_FPS);
}

unsigned int VideoFileSource::getGrabbedFrameCount()
{
  return this->i_grabbedFrameCount;
}

unsigned int VideoFileSource::getFrameCount()
{
  return this->o_capture->get(CV_CAP_PROP_FRAME_COUNT);
}

bool VideoFileSource::atEndOfVideo()
{
  return (this->getGrabbedFrameCount() >= this->getFrameCount());
}
