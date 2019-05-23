/*
 * Image Loader
 * Allan Legemaate
 * 21/05/2019
 * Abstract class defining interface to image loaders
 */

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <allegro.h>

class ImageLoader {
  public:
    ImageLoader();
    virtual ~ImageLoader();

    virtual int Load(const char* filename) = 0;

    uint32_t GetWidth();
    uint32_t GetHeight();
    uint8_t* GetRawData();
    BITMAP* GetBitmap();

  protected:
    virtual int LoadBitmap() = 0;

    uint32_t nWidth;
    uint32_t nHeight;
    uint8_t* pData;
    BITMAP* pImage;
};

#endif // IMAGELOADER_H
