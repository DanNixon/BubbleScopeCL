/**
 * \class VideoFileSource
 *
 * \brief Video file frame grabber
 *
 * Grabs frames and image information for a video file
 *
 * \author Dan Nixon
 */

#ifndef VIDEOFILESOURCE_H
#define VIDEOFILESOURCE_H

#include "FrameSource.h"

#include <stdlib.h>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class VideoFileSource: public FrameSource
{
  public:
    VideoFileSource();
    ~VideoFileSource();

    void open(std::string);
    void close();
    bool isOpen();
    bool grab(cv::Mat *);
    unsigned int getWidth();
    unsigned int getHeight();

    double getFrameRate();
    unsigned int getGrabbedFrameCount();
    unsigned  getFrameCount();
    bool atEndOfVideo();

  private:
    cv::VideoCapture *o_capture;
    unsigned int i_grabbedFrameCount;
};

#endif
