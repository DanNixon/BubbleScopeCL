#include "unwrap.h"

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

cv::Mat BubbleScopeUnwrapper::unwrap(cv::Mat *imageIn)
{
  cv::Mat imageOut = cv::Mat(this->i_unwrapHeight, this->i_unwrapWidth, CV_8UC3, cv::Scalar::all(0));
  unsigned char *unwrapPixels = imageOut.data;
  unsigned char *originalPixels = imageIn->data;

  int i;
  for(i = 0; i < this->i_outMatSize; i++)
    unwrapPixels[i] = originalPixels[this->ia_transformation[i]];

  return imageOut;
}

void BubbleScopeUnwrapper::unwrapWidth(int width)
{
  this->i_unwrapWidth = width;
  this->i_unwrapHeight = (int) (this->i_unwrapWidth / D_PI);
  this->i_outMatSize = this->i_unwrapWidth * this->i_unwrapHeight * 3;
}

void BubbleScopeUnwrapper::originalSize(int width, int height)
{
  this->i_originalWidth = width;
  this->i_originalHeight = height;
}

void BubbleScopeUnwrapper::originalCentre(float u, float v)
{
  this->f_uCentre = u;
  this->f_vCentre = v;
}

void BubbleScopeUnwrapper::imageRadius(float min, float max)
{
  this->f_radiusMin = min;
  this->f_radiusMax = max;
}

void BubbleScopeUnwrapper::offsetAngle(float angle)
{
  this->f_offsetAngle = angle;
}
