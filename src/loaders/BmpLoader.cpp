#include "loaders/BmpLoader.h"

int BmpLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_bitmap(filename, nullptr);
  SetDimensions();

  return (pImage != nullptr);
}
