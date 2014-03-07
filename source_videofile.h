/*
 * Video file frame grabber
 * Grabs frames and image information for a video file
 *
 * Dan Nixon
 */

#ifndef SVIDEOFILE_H
#define SVIDEOFILE_H

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
    int isOpen();
    void grab(cv::Mat *);
    int getWidth();
    int getHeight();

    int getFrameRate();
    unsigned int getGrabbedFrameCount();
    int getFrameCount();
    int atEndOfVideo();

  private:
    cv::VideoCapture *cap;
    unsigned int i_grabbedFrameCount;
};

#endif
