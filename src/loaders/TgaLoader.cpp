#include "loaders/TgaLoader.h"

int TgaLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_bitmap(filename, NULL);
  SetDimensions();

  return (pImage != nullptr);
}
