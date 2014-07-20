/**
 * \class TimelapseSource
 *
 * \brief Timelapse frame grabber
 *
 * Grabs still image frames using filename pattern with incrementing number
 *
 * \author Dan Nixon
 */

#ifndef TIMELAPSESOURCE_H
#define TIMELAPSESOURCE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "frame_source.h"

#include <opencv2/core/core.hpp>

class TimelapseSource: public FrameSource
{
  public:
    TimelapseSource();
    ~TimelapseSource();

    void open(std::string);
    void close();
    bool isOpen();
    bool grab(cv::Mat *);
    unsigned int getWidth();
    unsigned int getHeight();

  private:
    cv::Mat o_frame;
    char *filenamePattern;
    unsigned long frameNumber;
};

#endif
