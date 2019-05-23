/*
 * Pcx Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads PCX
 */

#ifndef PCXLOADER_H
#define PCXLOADER_H

#include "ImageLoader.h"

class PcxLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;
};

#endif // PCXLOADER_H
