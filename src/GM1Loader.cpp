#include "GM1Loader.h"

#include <iostream>
#include <fstream>
#include <vector>

// Convert GM1 token to name
std::string GM1Loader::data_type_name(int data_type) {
  switch (data_type) {
    case 1:
      return "interface items and buidling animations";
    case 2:
      return "animations";
    case 3:
      return "buildings";
    case 4:
      return "font";
    case 5:
    case 7:
      return "walls, grass, stone and others";
  }
  return "Invalid type (" + std::to_string(data_type) + ")";
}

// Load gm1 from file
BITMAP* GM1Loader::load_gm1(char const *filename, PALETTE pal) {
  // Create file
  std::ifstream f(filename, std::ios::binary | std::ios::ate);
  std::ifstream::pos_type pos = f.tellg();

  // Vector to dump file into
  std::vector<char> result(pos);

  // Read the file
  f.seekg(0, std::ios::beg);
  f.read(&result[0], pos);

  // Header
  unsigned int num_pictures = (unsigned int)result.at(12);
  unsigned int data_type = (unsigned int)result.at(20);
  unsigned int data_size = (unsigned int)result.at(80);

  std::cout << "num:" << num_pictures << " type:" << data_type_name(data_type) << " size:" << data_size << std::endl;

  return NULL;
}
