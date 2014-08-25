#include "VideoFileSource.h"

VideoFileSource::VideoFileSource() :
  m_capture(NULL),
  m_grabbedFrameCount(0)
{
}

VideoFileSource::~VideoFileSource()
{
  delete m_capture;
}

/**
 * \brief Creates a new video capture to read video file
 *
 * \param videoFile THe video file top open
 */
void VideoFileSource::open(std::string videoFile)
{
  m_capture = new cv::VideoCapture(videoFile);
  m_capture->set(CV_CAP_PROP_CONVERT_RGB, 1);
  m_grabbedFrameCount = 0;
}

/**
 * \brief Closes the open video file
 */
void VideoFileSource::close()
{
  m_capture->release();
}

bool VideoFileSource::isOpen()
{
  return m_capture->isOpened();
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
  bool result = m_capture->read(*out);
  if(result)
    m_grabbedFrameCount++;
  return result;
}

unsigned int VideoFileSource::getWidth()
{
  return m_capture->get(CV_CAP_PROP_FRAME_WIDTH);
}

unsigned int VideoFileSource::getHeight()
{
  return m_capture->get(CV_CAP_PROP_FRAME_HEIGHT);
}

/**
 * \brief Attempts to get the frame rate of the video
 *
 * \return Frame rate reported by OpenCV
 */
double VideoFileSource::getFrameRate()
{
  return m_capture->get(CV_CAP_PROP_FPS);
}

/**
 * \brief Gets the number of frames already captured from file
 *
 * \return Number of captured frames
 */
unsigned int VideoFileSource::getGrabbedFrameCount()
{
  return m_grabbedFrameCount;
}

/**
 * \brief Gets the total number of frames in the video file
 *
 * \return Number of rames in video
 */
unsigned int VideoFileSource::getFrameCount()
{
  return m_capture->get(CV_CAP_PROP_FRAME_COUNT);
}

/**
 * \brief Determines if the end of the video file has been reached
 *
 * \return True if at end of video, false otherwise
 */
bool VideoFileSource::atEndOfVideo()
{
  return (getGrabbedFrameCount() >= getFrameCount());
}
