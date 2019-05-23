#include "ImageLoader.h"

ImageLoader::ImageLoader() {
  nWidth = 0;
  nHeight = 0;
  pData = nullptr;
  pImage = nullptr;
  sLocation = "No Path!";
  nWHRatio = 0;
  nHWRatio = 0;
}

ImageLoader::~ImageLoader() {
  if (pData)
    delete pData;

  if (pImage)
    destroy_bitmap(pImage);
}

uint32_t ImageLoader::GetWidth() {
  return nWidth;
}

uint32_t ImageLoader::GetHeight() {
  return nHeight;
}

uint8_t* ImageLoader::GetRawData() {
  return pData;
}

BITMAP* ImageLoader::GetBitmap() {
  return pImage;
}

const char* ImageLoader::GetLocation() {
  return sLocation;
}

float ImageLoader::GetHWRatio() {
  return nHWRatio;
}

float ImageLoader::GetWHRatio() {
  return nWHRatio;
}

void ImageLoader::SetDimensions() {
  if (pImage) {
    nWidth = pImage -> w;
    nHeight = pImage -> h;
    nWHRatio = (float)nWidth / (float)nHeight;
    nHWRatio = (float)nHeight / (float)nWidth;
  }
}
