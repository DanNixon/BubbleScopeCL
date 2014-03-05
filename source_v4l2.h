
#ifndef SV4L2_H
#define SV4L2_H

#include <stdio.h>
#include <stdlib.h>

#include "frame_source.h"

#include <opencv2/core/core.hpp>
#include "OCVCapture.h"

class V4L2Source: public FrameSource
{
  public:
    V4L2Source();

    void open(std::string);
    void close();
    int isOpen();
    cv::Mat grab();
    int getWidth();
    int getHeight();

    void setCaptureSize(int, int);
    int getFrameRate();

  private:
    OCVCapture cap;
};

#endif
