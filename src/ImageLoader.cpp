#include "ImageLoader.h"

ImageLoader::ImageLoader() {
  nWidth = 0;
  nHeight = 0;
  pData = nullptr;
  pImage = nullptr;
}

ImageLoader::~ImageLoader() {

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