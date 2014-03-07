/*
 * Frame grabber
 * Grabs frames and image information for a specified source
 *
 * Dan Nixon
 */

#ifndef FRAMESOURCE_H
#define FRAMESOURCE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class FrameSource
{
  public:
    virtual void open(std::string) = 0;
    virtual void close() = 0;
    virtual int isOpen() = 0;
    virtual void grab(cv::Mat *) = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};

#endif
