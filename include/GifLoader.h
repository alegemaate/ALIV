/*
 * Gif Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads GIF
 */

#ifndef GIFLOADER_H
#define GIFLOADER_H

#include "ImageLoader.h"

class GifLoader: public ImageLoader {
  public:
    GifLoader();
    ~GifLoader();

    virtual int Load(const char* filename) override;
    virtual BITMAP* GetBitmap() override;
  private:
    BITMAP **frames;
    int *durations;
    unsigned int numFrames;
    int frame;
};

#endif // GIFLOADER_H
