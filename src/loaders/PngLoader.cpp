#include "loaders/PngLoader.h"

#include <loadpng.h>

int PngLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = load_png(filename, nullptr);
  SetDimensions();

  return (pImage != nullptr);
}
