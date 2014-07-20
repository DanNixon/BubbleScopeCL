/*
 * BubbleScope 360 image unwrap
 * Allows fast video processing/unwrapping using a pre-computed pixel
 * transformation array.
 * 
 * Dan Nixon
 */

#include "unwrap.h"

BubbleScopeUnwrapper::BubbleScopeUnwrapper()
{
  this->m_transformation = NULL;
}

BubbleScopeUnwrapper::~BubbleScopeUnwrapper()
{
  //Deallocate memory used for transformation array
  delete this->m_transformation;
}

/*
 * Computes the pixel transformation array.
 * All unwrap paremeters must be set beforehand.
 * Any changes to unwrap parameters must be followed by a call to this function
 * before any further calls to unwrap()
 */
bool BubbleScopeUnwrapper::generateTransformation()
{
  this->m_transformation = new unsigned long[this->m_outMatSize];

  float radius_delta = this->m_radiusMax - this->m_radiusMin;
  float aspect = (float) this->m_originalWidth / (float) this->m_originalHeight;

  unsigned int pixelSpan = this->m_originalWidth * this->m_radiusMax * 2;
  if(pixelSpan > this->m_originalHeight)
    return false;

  unsigned long index = 0;
  unsigned int i, j;
  for (i = this->m_unwrapHeight - 1; i > 0; i--)
  {
    float amplitutde = (radius_delta * (i / (float) this->m_unwrapHeight))
      + this->m_radiusMin;

    for (j = 0; j < this->m_unwrapWidth; ++j)
    {
      float longitudeAngle = (float) (D_PI * (j / (float) this->m_unwrapWidth))
        + this->m_offsetAngle;

      float sinLongAngle = sin(longitudeAngle);
      float cosLongAngle = cos(longitudeAngle);

      float u = aspect * sinLongAngle;
      float v = cosLongAngle;

      u *= amplitutde;
      v *= amplitutde;

      u += this->m_uCentre;
      v += (1.0f - this->m_vCentre);

      if(u > 1.0f)
        u = 1.0f;
      if(v > 1.0f)
        v = 1.0f;

      int xPixel = (int) ((1.0f - v) * this->m_originalWidth);
      int yPixel = (int) ((1.0f - u) * this->m_originalHeight);

      unsigned long oldPixelIndex =
        ((yPixel * this->m_originalWidth) + xPixel) * 3;

      this->m_transformation[index] = oldPixelIndex;
      this->m_transformation[index + 1] = oldPixelIndex + 1;
      this->m_transformation[index + 2] = oldPixelIndex + 2;

      index += 3;
    }
  }

  return true;
}

/*
 * Creates a 360 degree unwrap using the pre-computed array.
 * Must call generateTransformation() before calling this function.
 */
bool BubbleScopeUnwrapper::unwrap(cv::Mat *imageIn, cv::Mat **imageOut)
{
  if(!this->m_transformation)
    return false;

  *imageOut = new cv::Mat(this->m_unwrapHeight, this->m_unwrapWidth, CV_8UC3,
      cv::Scalar::all(0));
  unsigned char *unwrapPixels = (*imageOut)->data;
  unsigned char *originalPixels = imageIn->data;

  unsigned long i;
  for(i = 0; i < this->m_outMatSize; i++)
    unwrapPixels[i] = originalPixels[this->m_transformation[i]];

  return true;
}

/*
 * Width of unwrapped image.
 * Used to calculate height of unwrapped image and to allocate memory for
 * transformation array.
 */
bool BubbleScopeUnwrapper::unwrapWidth(int width)
{
  if(width <= 0)
    return false;

  this->m_unwrapWidth = width;
  this->m_unwrapHeight = (int) (this->m_unwrapWidth / D_PI);
  this->m_outMatSize = this->m_unwrapWidth * this->m_unwrapHeight * 3;

  return true;
}

/*
 * Sets the width and height of the original captured image.
 * Used to generate pixel transformation values.
 */
bool BubbleScopeUnwrapper::originalSize(int width, int height)
{
  if((width < 0) || (height < 0))
    return false;

  this->m_originalWidth = width;
  this->m_originalHeight = height;

  return true;
}

/*
 * Sets the centre of the image relative to it's dimensions.
 */
bool BubbleScopeUnwrapper::originalCentre(float u, float v)
{
  if((u < 0.0f) || (u > 1.0f))
    return false;
  if((v < 0.0f) || (v > 1.0f))
    return false;

  this->m_uCentre = u;
  this->m_vCentre = v;

  return true;
}

/*
 * Sets the upper and lower radii defining the section of the original image
 * to unwrap.
 */
bool BubbleScopeUnwrapper::imageRadius(float min, float max)
{
  if((min < 0.0f) || (min > 0.5f))
    return false;
  if((max < 0.0f) || (max > 0.5f))
    return false;
  if(min >= max)
    return false;

  this->m_radiusMin = min;
  this->m_radiusMax = max;

  return true;
}

/*
 * Sets offset angle for unwrapped image.
 * Equivalent of rotating BubbleScope on camera.
 */
bool BubbleScopeUnwrapper::offsetAngle(float angle)
{
  if((angle < 0.0f) || (angle > 360.0f))
    return false;

  this->m_offsetAngle = angle * DEG_2_RAD;

  return true;
}

/*
 * Gets the computed height of the unwrapped images.
 */
unsigned int BubbleScopeUnwrapper::getUnwrapHeight()
{
  return this->m_unwrapHeight;
}
