#include "parsers/GM1Parser.h"

#include <iostream>
#include <fstream>

#include "parsers/TGXParser.h"

// Convert GM1 token to name
std::string GM1Parser::data_type_name(int data_type) {
  switch (data_type) {
    case 1:
      return "interface items";
    case 2:
      return "animations";
    case 3:
      return "buildings";
    case 4:
      return "font";
    case 6:
      return "unknown animation";
    case 5:
    case 7:
      return "walls, grass, stone and others";
  }
  return "Invalid type (" + std::to_string(data_type) + ")";
}

// Convert 4 chars to int32
unsigned int GM1Parser::chars_to_int(char a, char b, char c, char d) {
  return (unsigned char)a | ((unsigned char)b << 8) | ((unsigned char)c << 16) | ((unsigned char)d << 24);
}

// Load animation
BITMAP* GM1Parser::load_gm1_animation(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data, std::vector<unsigned int> *pall) {
	return TGXParser::load_tgx_helper(bytes, iter, image_data -> width, image_data -> height, pall);
}

// Load tile
BITMAP* GM1Parser::load_gm1_tile(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data) {
  // File iterator and image x and y
	unsigned int pixels_per_line = 2;
	unsigned int img_size = 256;
	unsigned int x = 14;
	unsigned int y = 0;

	// Make bitmap
  BITMAP *bmp = create_bitmap(image_data -> width, image_data -> height);
  clear_to_color(bmp, makecol(255,255,255));

	// Parse file
	for (unsigned int t = 0; t < img_size; t++) {
    // Break if we go too far
    if (*iter + 1 > bytes -> size())
      break;

    putpixel(bmp, x, y, TGXParser::convert_color((unsigned char)bytes -> at(*iter), (unsigned char)bytes -> at(*iter + 1)));
    x ++;
    *iter += 2;
    if (x > 14 + pixels_per_line / 2) {
      if (y < 7)
        pixels_per_line += 4;
      else if (y > 7)
        pixels_per_line -= 4;

      y += 1;
      x = 15 - pixels_per_line / 2;
    }
	}

	// Return bmp
  return bmp;
}

// Load tgx
BITMAP* GM1Parser::load_gm1_tgx(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data) {
  return TGXParser::load_tgx_helper(bytes, iter, image_data -> width, image_data -> height);
}

// Load uncompressed
BITMAP* GM1Parser::load_gm1_uncompressed(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data) {
	// File iterator and image x and y
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int img_size = image_data -> width * image_data -> height;

	// Make bitmap
  BITMAP *bmp = create_bitmap(image_data -> width, image_data -> height);
  clear_to_color(bmp, makecol(255,255,255));

	// Parse file
	for (unsigned int t = 0; t < img_size; t++) {
    // Break if we go too far
    if (*iter + 1 > bytes -> size())
      break;

    putpixel(bmp, x, y, TGXParser::convert_color((unsigned char)bytes -> at(*iter), (unsigned char)bytes -> at(*iter + 1)));
    x ++;
    if (x == image_data -> width) {
      y += 1;
      x = 0;
    }
    *iter += 2;
	}

	// Return bmp
  return bmp;
}

// Load gm1 from file
std::vector<BITMAP*> GM1Parser::load_gm1(char const *filename, PALETTE pal) {
  // Create file
  std::ifstream f(filename, std::ios::binary | std::ios::ate);
  std::ifstream::pos_type pos = f.tellg();

  // Vector to dump file into
  std::vector<char> result(pos);

  // Vector to store offsets
  std::vector<GM1Data> image_data;

  // Return bitmaps
  std::vector<BITMAP*> return_bitmaps;

  // Read the file
  f.seekg(0, std::ios::beg);
  f.read(&result[0], pos);

  // Header
  unsigned int num_pictures = (unsigned char)result.at(12) + (unsigned char)result.at(13) * 256;
  unsigned int data_type = (unsigned char)result.at(20);

  // Iterator, skip header
  unsigned int i = 88;

  // Create pallette
  std::vector<unsigned int> pall;

  // Only animations use pallette
  if (data_type == 2) {
    // Load pallete (constant size)
    for (unsigned int t = i; i < t + 5120; i += 2) {
      pall.push_back(TGXParser::convert_color(result.at(i), result.at(i + 1)));
    }
  }

  // Iterator, skip pallete
  i = 5120 + 88;

  // Create empty image data
  for (unsigned int i = 0; i < num_pictures; i++) {
    GM1Data newData;
    newData.index = i;
    image_data.push_back(newData);
  }

  // Get image offset list
  for (unsigned int t = i; i < t + (num_pictures * 4); i += 4) {
    image_data.at((i - t) / 4).offset = chars_to_int(result.at(i), result.at(i + 1), result.at(i + 2), result.at(i + 3));
    //std::cout << "Image " << (i - t) / 4  << " offset: " << image_data.at((i - t) / 4).offset << std::endl;
  }

  // Get image size list
  for (unsigned int t = i; i < t + (num_pictures * 4); i += 4) {
    image_data.at((i - t) / 4).size = chars_to_int(result.at(i), result.at(i + 1), result.at(i + 2), result.at(i + 3));
    //std::cout << "Image " << (i - t) / 4  << " size: " << image_data.at((i - t) / 4).size << std::endl;
  }

  // Image header
  for (unsigned int t = i; i < t + (num_pictures * 16); i += 16) {
    image_data.at((i - t) / 16).width = (unsigned char)result.at(i) + (unsigned char)result.at(i + 1) * 256;
    image_data.at((i - t) / 16).height = (unsigned char)result.at(i + 2) + (unsigned char)result.at(i + 3) * 256;
    /*std::cout << "Image " << (i - t) / 16
              << " width: " << image_data.at((i - t) / 16).width
              << " height: " << image_data.at((i - t) / 16).height << std::endl;*/
  }

  // Go through each image
  for (unsigned int t = 0; t < image_data.size(); t++) {
    // Image data offset
    unsigned int new_iter = i + image_data.at(t).offset;
    //std::cout << "Loading image " << image_data.at(t).index << " at " << new_iter << std::endl;

    // Split between many gm1 types
    switch (data_type) {
      case 1:
      case 4:
      case 6:
        image_data.at(t).image = load_gm1_tgx(&result, &new_iter, &image_data.at(t));
        break;
      case 2:
        image_data.at(t).image = load_gm1_animation(&result, &new_iter, &image_data.at(t), &pall);
        break;
      case 3:
        image_data.at(t).image = load_gm1_tile(&result, &new_iter, &image_data.at(t));
        break;
      case 5:
      case 7:
        image_data.at(t).image = load_gm1_uncompressed(&result, &new_iter, &image_data.at(t));
        break;
      default:
        std::cout << "Invalid data type (" << data_type << ")" << std::endl;
        image_data.at(t).image = nullptr;
    }

    // Add bitmap to return vector
    if (image_data.at(t).image) {
      return_bitmaps.push_back(image_data.at(t).image);
    }
  }

  return return_bitmaps;
}
