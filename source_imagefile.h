
#ifndef SIMAGEFILE_H
#define SIMAGEFILE_H

#include <stdlib.h>

#include "frame_source.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class StillImageSource : public FrameSource
{
  public:
    void open(std::string);
    void close();
    int isOpen();
    cv::Mat grab();
    int getWidth();
    int getHeight();

  private:
    cv::Mat image;
};

#endif
