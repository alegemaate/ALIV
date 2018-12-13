#include "TGXLoader.h"

#include <fstream>
#include <iostream>

// Convert tgx token to name
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

// Convert 15 bit colour to 24 bit
int TGXLoader::convert_color(unsigned char byte1, unsigned char byte2) {
  // Extract values
  unsigned char r = byte2 >> 2;
  unsigned char g = ((byte1 >> 5) & 0b00000111) | ((byte2 << 3) & 0b00011000);
  unsigned char b = byte1 & 0b00011111;

  // Convert from 15 bit to 24
  // Technically this will truncate the whites
  //   but I can't think of a better solition
  //   without involving float rounding.
  r = r * 8;
  g = g * 8;
  b = b * 8;

  // Return new colour
  return makecol(r, g, b);
}

// Convert 15 bit colour to 24 bit
int TGXLoader::pallete_lookup(unsigned char addr, std::vector<unsigned int>* pall) {
  if (addr < pall -> size()) {
    return pall -> at(addr);
  }
  return makecol(255, 0, 0);
}

// Tgx helper used by file and memory
BITMAP* TGXLoader::load_tgx_helper(std::vector<char> *bytes, unsigned int *iter, unsigned int width, unsigned int height, std::vector<unsigned int>* pall) {
  // Make bitmap
  BITMAP *bmp = create_bitmap_ex(24, width, height);
  clear_to_color(bmp, makecol(255, 255, 255));

	// File iterator and image x and y
	unsigned int x = 0;
	unsigned int y = 0;

	// Parse file
	while (*iter < bytes -> size()) {
    // Close
    if (y >= height)
      break;

    // Extract token and length
    int token = (unsigned char)bytes -> at(*iter) >> 5;
    int length = ((unsigned char)bytes -> at(*iter) & 0b00011111) + 1;

    // Deal with tokens accordingly
    switch (token) {
      // Pixel stream
      case 0:
        *iter += 1;
        for (unsigned int t = x + length; x < t; x++) {
          // 15 bit colour
          if (pall == nullptr) {
            putpixel(bmp, x, y, convert_color((unsigned char)bytes -> at(*iter), (unsigned char)bytes -> at(*iter + 1)));
            *iter += 2;
          }
          // Pallette lookup
          else {
            putpixel(bmp, x, y, pallete_lookup((unsigned char)bytes -> at(*iter), pall));
            *iter += 1;
          }
        }
        break;
      // Transparent pixels
      case 1:
        *iter += 1;
        for (unsigned int t = x + length; x < t; x++) {
          putpixel(bmp, x, y, makecol(255, 0, 255));
        }
        break;
      // Repeating pixels
      case 2:
        *iter += 1;
        // 15 bit colour
        if (pall == nullptr) {
          for (unsigned int t = x + length; x < t; x++) {
            putpixel(bmp, x, y, convert_color((unsigned char)bytes -> at(*iter), (unsigned char)bytes -> at(*iter + 1)));
          }
          *iter += 2;
        }
        // Pallette lookup
        else {
          for (unsigned int t = x + length; x < t; x++) {
            putpixel(bmp, x, y, pallete_lookup((unsigned char)bytes -> at(*iter), pall));
          }
          *iter += 1;
        }
        break;
      // New line
      case 4:
        // Fill rest of line
        for (; x < width; x++) {
          putpixel(bmp, x, y, makecol(255, 0, 255));
        }
        // New line
        y += 1;
        x = 0;
        *iter += 1;
        for (unsigned int t = length - 1; x < t; x++) {
          putpixel(bmp, x, y, makecol(255, 0, 255));
        }
        break;
      // Should never get here
      default:
        std::cout << "Invalid token (" << token << ") at " << *iter << " length " << length << std::endl;
        *iter += 1;
        break;
    }
	}

	// Return bmp
	return bmp;
}

// Load tgx from file
BITMAP* TGXLoader::load_tgx(char const *filename, PALETTE pal) {
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

  // Iterator start position
  unsigned int iter = 8;

  // Return new image
  return load_tgx_helper(&result, &iter, width, height);
}
