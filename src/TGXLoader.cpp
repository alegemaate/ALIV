#include "TGXLoader.h"

#include <iostream>
#include <fstream>
#include <vector>

TGXLoader::TGXLoader() {

}

TGXLoader::~TGXLoader() {

}

std::string TGXLoader::token_name(int token) {
  switch (token) {
    case 0:
      return "Stream of pixels";
    case 1:
      return "Transparent pixels";
    case 2:
      return "Repeating pixels";
    case 4:
      return "New line";
  }
  return "Invalid token";
}

int TGXLoader::convert_color(unsigned char byte1, unsigned char byte2) {
  unsigned char r = byte2 >> 2;
  unsigned char g = ((byte1 >> 5) & 0b00000111) | ((byte2 << 3) & 0b00011000);
  unsigned char b = byte1 & 0b00011111;

  return makecol(r * 8, g * 8, b * 8);
}

BITMAP* TGXLoader::load_tgx(char const *filename, PALETTE pal) {
  // Debug mode
  bool debug = false;

  // Create file
  std::ifstream f(filename, std::ios::binary | std::ios::ate);
  std::ifstream::pos_type pos = f.tellg();

  // Vector to dump file into
  std::vector<char> result(pos);

  // Read the file
  f.seekg(0, std::ios::beg);
  f.read(&result[0], pos);

  // Header
  int width = (unsigned char)result.at(0) + 256 * (unsigned char)result.at(1);
  int height = (unsigned char)result.at(4) + 256 * (unsigned char)result.at(5);
  std::cout << "width: " << width << std::endl;
  std::cout << "height: " << height << std::endl;

  // Make bitmap
  BITMAP *bmp = create_bitmap_ex(24, width, height);
  clear_to_color(bmp, makecol(255,255,255));

	// Print data
	unsigned int i = 8;
	unsigned int x = 0;
	unsigned int y = 0;
	while (i < result.size()) {
    int token = (((unsigned char)result.at(i)) >> 5);
    int length = (((unsigned char)result.at(i)) & 0b00011111) + 1;

    if (debug) {
      std::cout << "Token: " << token_name(token) << "(" << token << ")" << " Length: " << length << std::endl;
    }

    switch (token) {
      // Pixel stream
      case 0:
        i += 1;
        for (unsigned int t = x + length; x < t; x++) {
          putpixel(bmp, x, y, convert_color((unsigned char)result.at(i), (unsigned char)result.at(i + 1)));
          i += 2;
        }
        break;
      // Transparent pixels
      case 1:
        i += 1;
        for (unsigned int t = x + length; x < t; x++) {
          putpixel(bmp, x, y, makecol(255, 0, 255));
        }
        break;
      // Repeating pixels
      case 2:
        i += 1;
        for (unsigned int t = x + length; x < t; x++) {
          putpixel(bmp, x, y, convert_color((unsigned char)result.at(i), (unsigned char)result.at(i + 1)));
        }
        i += 2;
        break;
      // New line
      case 4:
        y += 1;
        x = 0;
        i += 1;
        for (unsigned int t = length - 1; x < t; x++) {
          putpixel(bmp, x, y, makecol(255, 0, 255));
        }
        break;
      default:
        std::cout << "Invalid Token";
        break;
    }
	}

  // Close image
  return bmp;
}
