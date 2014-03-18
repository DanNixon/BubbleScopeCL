/*
 * Timelapse frame grabber
 * Grabs still image frames using filename pattern with incrementing number
 *
 * Dan Nixon
 */

#ifndef TIMELAPSESOURCE_H
#define TIMELAPSESOURCE_H

class TimelapseSource: public FrameSource
{
  public:
    void open(std::string);
    void close();
    bool isOpen();
    void grab(cv::Mat *);
    unsigned long getWidth();
    unsigned long getHeight();

  private:
    unsigned long frameNumber;
};

#endif
