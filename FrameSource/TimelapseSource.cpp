#include "TimelapseSource.h"

TimelapseSource::TimelapseSource():
  m_filenamePattern(NULL),
  m_frameNumber(0)
{
}

TimelapseSource::~TimelapseSource()
{
  delete m_filenamePattern;
}

/**
 * \brief Sets filename pattern for timelapse frames
 *
 * \param path The path with filename pattern
 */
void TimelapseSource::open(std::string path)
{
  m_filenamePattern = (char *) path.c_str();
  m_frameNumber = 0;
}

void TimelapseSource::close()
{
  m_filenamePattern = NULL;
  m_frameNumber = 0;
}

bool TimelapseSource::isOpen()
{
  return (m_filenamePattern != NULL);
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
  unsigned int filenameLen = strlen(m_filenamePattern) + 5;
  char *imageFilename = new char[filenameLen];
  sprintf(imageFilename, m_filenamePattern, m_frameNumber);
  
  struct stat buffer;   
  if(stat (imageFilename, &buffer))
    return false;

  m_frame = cv::imread(imageFilename);
  *out = m_frame;

  //Increment frame number
  m_frameNumber++;

  return true;
}

unsigned int TimelapseSource::getWidth()
{
  cv::Mat frame;
  grab(&frame);
  m_frameNumber--;
  return frame.cols;
}

unsigned int TimelapseSource::getHeight()
{
  cv::Mat frame;
  grab(&frame);
  m_frameNumber--;
  return frame.rows;
}
