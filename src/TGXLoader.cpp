#include "TgxLoader.h"

#include "TGXParser.h"

int TgxLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = TGXParser::load_tgx(filename, NULL);
  SetDimensions();

  return (pImage != nullptr);
}
