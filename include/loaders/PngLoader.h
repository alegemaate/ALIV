/*
 * Png Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads PNG
 */

#ifndef PNGLOADER_H
#define PNGLOADER_H

#include "ImageLoader.h"

class PngLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;
};

#endif // PNGLOADER_H
