/**
 * \class FrameSource
 *
 * \brief Frame grabber
 *
 * Grabs frames and image information for a specified source
 *
 * \author Dan Nixon
 */

#ifndef FRAMESOURCE_H
#define FRAMESOURCE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class FrameSource
{
  public:
    /**
     * \brief Attempt to open the source at a given location
     *
     * \param location Location to open source
     */
    virtual void open(std::string location) = 0;

    /**
     * \brief Close the frame source
     */
    virtual void close() = 0;

    /**
     * \brief Checks to see if the source is open
     *
     * \return True if opened, false otherwise
     */
    virtual bool isOpen() = 0;

    /**
     * \brief Attempts to grab a frame from the source
     *
     * \param out The matrix to save the frame to
     *
     * \rteurn True on successful capture, false otherwise
     */
    virtual bool grab(cv::Mat * out) = 0;

    /**
     * \brief Gets the width of the captured frames
     *
     * \return Frame width
     */
    virtual unsigned int getWidth() = 0;

    /**
     * \brief Gets the height of the captured frames
     *
     * \return Frame height
     */
    virtual unsigned int getHeight() = 0;
};

#endif
