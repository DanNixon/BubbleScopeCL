/*
 * BubbleScope 360 image unwrap
 * Allows fast video processing/unwrapping using a pre-computed pixel transformation array.
 * 
 * Dan Nixon
 */

#include "unwrap.h"

BubbleScopeUnwrapper::BubbleScopeUnwrapper()
{
  this->ia_transformation = NULL;
}

BubbleScopeUnwrapper::~BubbleScopeUnwrapper()
{
  delete this->ia_transformation;
}

/*
 * Computes the pixel transformation array.
 * All unwrap paremeters must be set beforehand.
 * Any changes to unwrap parameters must be followed by a call to this function before any further calls to unwrap()
 */
void BubbleScopeUnwrapper::generateTransformation()
{
  this->ia_transformation = new int[this->i_outMatSize];

  float radius_delta = this->f_radiusMax - this->f_radiusMin;
  float aspect = (float) this->i_originalWidth / (float) this->i_originalHeight;

  int index = 0;
  int i, j;
  for (i = this->i_unwrapHeight - 1; i > 0; i--)
  {
    float amplitutde = (radius_delta * (i / (float) this->i_unwrapHeight)) + this->f_radiusMin;
    for (j = 0; j < this->i_unwrapWidth; ++j)
    {
      float longitudeAngle = (float) (D_PI * (j / (float) this->i_unwrapWidth)) + this->f_offsetAngle;

      float sinLongAngle = sin(longitudeAngle);
      float cosLongAngle = cos(longitudeAngle);

      float u = aspect * sinLongAngle;
      float v = cosLongAngle;

      u *= amplitutde;
      v *= amplitutde;

      u += this->f_uCentre;
      v += (1.0f - this->f_vCentre);

      int xPixel = (int) ((1 - v) * this->i_originalWidth);
      int yPixel = (int) ((1 - u) * this->i_originalHeight);

      int oldPixelIndex = ((yPixel * this->i_originalWidth) + xPixel) * 3;

      this->ia_transformation[index] = oldPixelIndex;
      this->ia_transformation[index + 1] = oldPixelIndex + 1;
      this->ia_transformation[index + 2] = oldPixelIndex + 2;

      index += 3;
    }
  }
}

/*
 * Creates a 360 degree unwrap using the pre-computed array.
 * Must call generateTransformation() before calling this function.
 */
cv::Mat BubbleScopeUnwrapper::unwrap(cv::Mat *imageIn)
{
  assert(this->ia_transformation != NULL);

  cv::Mat imageOut(this->i_unwrapHeight, this->i_unwrapWidth, CV_8UC3, cv::Scalar::all(0));
  unsigned char *unwrapPixels = imageOut.data;
  unsigned char *originalPixels = imageIn->data;

  int i;
  for(i = 0; i < this->i_outMatSize; i++)
    unwrapPixels[i] = originalPixels[this->ia_transformation[i]];

  return imageOut;
}

void BubbleScopeUnwrapper::unwrapWidth(int width)
{
  assert(width > 0);
  this->i_unwrapWidth = width;
  this->i_unwrapHeight = (int) (this->i_unwrapWidth / D_PI);
  this->i_outMatSize = this->i_unwrapWidth * this->i_unwrapHeight * 3;
}

void BubbleScopeUnwrapper::originalSize(int width, int height)
{
  assert(width > 0);
  assert(height > 0);
  this->i_originalWidth = width;
  this->i_originalHeight = height;
}

void BubbleScopeUnwrapper::originalCentre(float u, float v)
{
  assert((u >= 0.0f) && (u <= 1.0f));
  assert((v >= 0.0f) && (v <= 1.0f));
  this->f_uCentre = u;
  this->f_vCentre = v;
}

void BubbleScopeUnwrapper::imageRadius(float min, float max)
{
  assert((min >= 0.0f) && (min <= 1.0f));
  assert((max >= 0.0f) && (max <= 1.0f));
  assert(min < max);
  this->f_radiusMin = min;
  this->f_radiusMax = max;
}

void BubbleScopeUnwrapper::offsetAngle(float angle)
{
  assert((angle >= 0.0f) && (angle <= D_PI)); //TODO: Is this range correct?
  this->f_offsetAngle = angle;
}
