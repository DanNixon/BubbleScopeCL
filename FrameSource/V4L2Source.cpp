#include "V4L2Source.h"

V4L2Source::V4L2Source() :
  m_capture(new OCVCapture())
{
  m_capture->setVerbose(false);
}

V4L2Source::~V4L2Source()
{
  close();
  delete m_capture;
}

/**
 * \brief Attempts to open the V4L2 device
 *
 * \param deviceName The V4L2 device to open
 */
void V4L2Source::open(std::string deviceName)
{
  m_capture->open(deviceName.c_str());
}

void V4L2Source::close()
{
  m_capture->close();
}

bool V4L2Source::isOpen()
{
  return m_capture->isOpen();
}

/**
 * \brief Attempt to grab a frame from the V4L2 device
 *
 * \param out The matrix for the captured frame
 *
 * \return True on successful capture, false otherwise
 */
bool V4L2Source::grab(cv::Mat *out)
{
  m_capture->grab();
  m_capture->rgb(*out);
  return true;
}

unsigned int V4L2Source::getWidth()
{
  return m_capture->width();
}

unsigned int V4L2Source::getHeight()
{
  return m_capture->height();
}

/**
 * \brief Set the desired capture size for the V4L2 device
 *
 * \param width Desired frame width
 *
 * \param height Desired frame height
 */
void V4L2Source::setCaptureSize(unsigned int width, unsigned int height)
{
  m_capture->setDesiredSize(width, height);
}

/**
 * \brief Get the frame rate reported by the V4L2 device
 *
 * \return Reported frame rate
 */
double V4L2Source::getFrameRate()
{
  return m_capture->frameRate();
}
