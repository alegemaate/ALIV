#include "loaders/Gm1Loader.h"

#include "parsers/GM1Parser.h"

Gm1Loader::Gm1Loader()
  : frame(0), lastTick(std::chrono::steady_clock::now()) {

}


Gm1Loader::~Gm1Loader() {
  // TODO: remove frames and durations
}

int Gm1Loader::Load(const char* filename) {
  sLocation = filename;
  frames = GM1Parser::load_gm1(filename);

  if(frames.at(0)) {
    pImage = frames.at(0);
    SetDimensions();
  }

  return (pImage != nullptr);
}

BITMAP* Gm1Loader::GetBitmap() {
  std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();

  if(std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count() > 50) {
    lastTick = now;
    frame = (frame + 1) % frames.size();
    pImage = frames.at(frame);
    SetDimensions();
  }

  return pImage;
}
