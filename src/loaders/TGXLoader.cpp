#include "loaders/TgxLoader.h"
#include "parsers/TGXParser.h"

int TgxLoader::Load(const char* filename) {
  sLocation = filename;
  pImage = TGXParser::load_tgx(filename);
  SetDimensions();

  return (pImage != nullptr);
}
