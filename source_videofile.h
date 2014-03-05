
#ifndef SVIDEOFILE_H
#define SVIDEOFILE_H

#include <stdlib.h>

#include "frame_source.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class VideoSource: public FrameSource
{
  public:
    void open(std::string);
    void close();
    int isOpen();
    void grab(cv::Mat *);
    int getWidth();
    int getHeight();

    int getFrameRate();
    int getImageType();

  private:
    cv::VideoCapture cap;
};

#endif
