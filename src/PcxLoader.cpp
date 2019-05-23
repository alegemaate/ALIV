#include "PcxLoader.h"

int PcxLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_bitmap(filename, NULL);

  return (pImage != nullptr);
}
