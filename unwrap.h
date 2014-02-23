/*
 * BubbleScope 360 image unwrap
 * Allows fast video processing/unwrapping using a pre-computed pixel transformation array.
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

    void generateTransformation();
    cv::Mat unwrap(cv::Mat *);

    unsigned int getUnwrapHeight();

    //Unwrap parameter setters
    void unwrapWidth(int);
    void originalSize(int, int);
    void originalCentre(float, float);
    void imageRadius(float, float);
    void offsetAngle(float);

  private:
    //Pixel transformation array
    int *ia_transformation;

    //Unwrap parameters
    unsigned int i_originalWidth;
    unsigned int i_originalHeight;
    unsigned int i_unwrapWidth;
    float f_uCentre;
    float f_vCentre;
    float f_radiusMin;
    float f_radiusMax;
    float f_offsetAngle;

    //Computed unwrap parameters
    unsigned int i_unwrapHeight;
    unsigned long i_outMatSize;
};

#endif
