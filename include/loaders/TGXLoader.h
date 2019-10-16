/*
 * Tgx Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads TGX
 */

#ifndef TGXLOADER_H
#define TGXLOADER_H

#include "ImageLoader.h"

class TgxLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;
};

#endif // TGXLOADER_H
