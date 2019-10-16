#include "loaders/ImageLoader.h"

ImageLoader::ImageLoader() {
  nWidth = 0;
  nHeight = 0;
  pData = nullptr;
  pImage = nullptr;
  sLocation = "No Path!";
}

ImageLoader::~ImageLoader() {
  if(pData)
    delete pData;

  if(pImage)
    destroy_bitmap(pImage);
}

uint32_t ImageLoader::GetWidth() const {
  return nWidth;
}

uint32_t ImageLoader::GetHeight() const {
  return nHeight;
}

uint8_t* ImageLoader::GetRawData() const {
  return pData;
}

BITMAP* ImageLoader::GetBitmap() {
  return pImage;
}

const char* ImageLoader::GetLocation() const {
  return sLocation;
}

float ImageLoader::GetHWRatio() const {
  return (float)nHeight / (float)nWidth;
}

float ImageLoader::GetWHRatio() const {
  return (float)nWidth / (float)nHeight;
}

void ImageLoader::SetDimensions() {
  if(pImage) {
    nWidth = pImage -> w;
    nHeight = pImage -> h;
  }
}
