/*
 * Bmp Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads BMP
 */

#ifndef BMPLOADER_H
#define BMPLOADER_H

#include "ImageLoader.h"

class BmpLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;
};

#endif // BMPLOADER_H
