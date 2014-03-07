/*
 * Still image frame grabber
 * Grabs still image frame and image information from file
 *
 * Dan Nixon
 */

#ifndef SIMAGEFILE_H
#define SIMAGEFILE_H

#include <stdlib.h>
#include <stdio.h>

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
    void grab(cv::Mat *);
    int getWidth();
    int getHeight();

  private:
    cv::Mat image;
    int i_open;
};

#endif
