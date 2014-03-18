/*
 * Video file frame grabber
 * Grabs frames and image information for a video file
 *
 * Dan Nixon
 */

#ifndef VIDEOFILESOURCE_H
#define VIDEOFILESOURCE_H

#include <stdlib.h>
#include <stdio.h>

#include "frame_source.h"

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
