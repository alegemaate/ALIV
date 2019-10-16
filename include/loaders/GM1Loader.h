/*
 * Gm1 Loader
 * Allan Legemaate
 * 22/05/2019
 * Loads GM1
 */

#ifndef GM1LOADER_H
#define GM1LOADER_H

#include "ImageLoader.h"

#include <chrono>
#include <vector>

class Gm1Loader: public ImageLoader {
  public:
    Gm1Loader();
    ~Gm1Loader();

    virtual int Load(const char* filename) override;
    virtual BITMAP* GetBitmap() override;
  private:
    int frame;
    std::chrono::time_point<std::chrono::steady_clock> lastTick;

    std::vector<BITMAP*> frames;
};

#endif // GM1LOADER_H
