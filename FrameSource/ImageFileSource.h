/**
 * \class ImageFileSource
 *
 * \brief Still image frame grabber
 *
 * Grabs still image frame and image information from file
 *
 * \author Dan Nixon
 */

#ifndef IMAGEFILESOURCE_H
#define IMAGEFILESOURCE_H

#include "FrameSource.h"

#include <stdlib.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ImageFileSource : public FrameSource
{
  public:
    ImageFileSource();

    void open(std::string);
    void close();
    bool isOpen();
    bool grab(cv::Mat *);
    unsigned int getWidth();
    unsigned int getHeight();

  private:
    cv::Mat m_image;
    bool m_open;
};

#endif
