#include "loaders/PcxLoader.h"

int PcxLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_bitmap(filename, nullptr);
  SetDimensions();

  return (pImage != nullptr);
}
