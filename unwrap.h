/*
 * BubbleScope 360 image unwrap
 * Allows fast video processing/unwrapping using a pre-computed pixel
 * transformation array.
 * 
 * Dan Nixon
 */

#ifndef UNWRAP_H
#define UNWRAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define D_PI 6.28318530718
#define DEG_2_RAD 0.0174532925

class BubbleScopeUnwrapper
{
  public:
    BubbleScopeUnwrapper();
    ~BubbleScopeUnwrapper();

    bool generateTransformation();
    bool unwrap(cv::Mat *, cv::Mat **);

    unsigned int getUnwrapHeight();

    //Unwrap parameter setters
    bool unwrapWidth(int);
    bool originalSize(int, int);
    bool originalCentre(float, float);
    bool imageRadius(float, float);
    bool offsetAngle(float);

  private:
    //Pixel transformation array
    unsigned long *m_transformation;

    //Unwrap parameters
    unsigned int m_originalWidth;
    unsigned int m_originalHeight;
    unsigned int m_unwrapWidth;
    float m_uCentre;
    float m_vCentre;
    float m_radiusMin;
    float m_radiusMax;
    float m_offsetAngle;

    //Computed unwrap parameters
    unsigned int m_unwrapHeight;
    unsigned long m_outMatSize;
};

#endif
