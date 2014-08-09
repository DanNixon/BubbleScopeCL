#include "VideoFileSource.h"

VideoFileSource::VideoFileSource()
{
  this->o_capture = NULL;
  this->i_grabbedFrameCount = 0;
}

VideoFileSource::~VideoFileSource()
{
  delete this->o_capture;
}

/**
 * \brief Creates a new video capture to read video file
 *
 * \param videoFile THe video file top open
 */
void VideoFileSource::open(std::string videoFile)
{
  this->o_capture = new cv::VideoCapture(videoFile);
  this->o_capture->set(CV_CAP_PROP_CONVERT_RGB, 1);
  this->i_grabbedFrameCount = 0;
}

/**
 * \brief Closes the open video file
 */
void VideoFileSource::close()
{
  this->o_capture->release();
}

bool VideoFileSource::isOpen()
{
  return this->o_capture->isOpened();
}

/**
 * \brief Attempts to capture a frame from the open video file
 *
 * \param out The matrix for the captured frame
 *
 * \return True on successful capture, false otherwise
 */
bool VideoFileSource::grab(cv::Mat *out)
{
  bool result = this->o_capture->read(*out);
  if(result)
    this->i_grabbedFrameCount++;
  return result;
}

unsigned int VideoFileSource::getWidth()
{
  return this->o_capture->get(CV_CAP_PROP_FRAME_WIDTH);
}

unsigned int VideoFileSource::getHeight()
{
  return this->o_capture->get(CV_CAP_PROP_FRAME_HEIGHT);
}

/**
 * \brief Attempts to get the frame rate of the video
 *
 * \return Frame rate reported by OpenCV
 */
double VideoFileSource::getFrameRate()
{
  return this->o_capture->get(CV_CAP_PROP_FPS);
}

/**
 * \brief Gets the number of frames already captured from file
 *
 * \return Number of captured frames
 */
unsigned int VideoFileSource::getGrabbedFrameCount()
{
  return this->i_grabbedFrameCount;
}

/**
 * \brief Gets the total number of frames in the video file
 *
 * \return Number of rames in video
 */
unsigned int VideoFileSource::getFrameCount()
{
  return this->o_capture->get(CV_CAP_PROP_FRAME_COUNT);
}

/**
 * \brief Determines if the end of the video file has been reached
 *
 * \return True if at end of video, false otherwise
 */
bool VideoFileSource::atEndOfVideo()
{
  return (this->getGrabbedFrameCount() >= this->getFrameCount());
}
