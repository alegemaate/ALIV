#include "GifLoader.h"

#include "algif/algif.h"

GifLoader::GifLoader() {
  frames = nullptr;
  durations = nullptr;
  numFrames = 0;
  frame = 0;
}


GifLoader::~GifLoader() {
  // TODO: remove frames and durations
}

int GifLoader::Load(const char* filename) {
  sLocation = filename;
  numFrames = algif_load_animation (filename, &frames, &durations);
  pImage = frames[0];
  SetDimensions();

  return (pImage != nullptr);
}

BITMAP* GifLoader::GetBitmap() {
  frame = (frame + 1) % numFrames;
  pImage = frames[frame];

  return pImage;
}
