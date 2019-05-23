#include "TgaLoader.h"

int TgaLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_bitmap(filename, NULL);

  return (pImage != nullptr);
}
