#include "GifLoader.h"

#include "algif/algif.h"

GifLoader::GifLoader() {
  frames = nullptr;
  durations = nullptr;
  numFrames = 0;
  frame = 0;
  lastTick = std::chrono::steady_clock::now();
}


GifLoader::~GifLoader() {
  // TODO: remove frames and durations
}

int GifLoader::Load(const char* filename) {
  sLocation = filename;
  numFrames = algif_load_animation (filename, &frames, &durations);

  if (frames[0]) {
    pImage = frames[0];
    SetDimensions();
  }

  return (pImage != nullptr);
}

BITMAP* GifLoader::GetBitmap() {
  std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
  if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count() > durations[frame] * 10) {
    lastTick = now;
    frame = (frame + 1) % numFrames;
    pImage = frames[frame];
  }

  return pImage;
}
