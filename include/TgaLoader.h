/*
 * Tga Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads TGA
 */

#ifndef TGALOADER_H
#define TGALOADER_H

#include "ImageLoader.h"

class TgaLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;
};

#endif // TGALOADER_H
