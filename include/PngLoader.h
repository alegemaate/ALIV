#ifndef PNGLOADER_H
#define PNGLOADER_H

#include "ImageLoader.h"

class PngLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;
};

#endif // PNGLOADER_H
