#include "BmpLoader.h"

int BmpLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_bitmap(filename, NULL);
  SetDimensions();

  return (pImage != nullptr);
}
