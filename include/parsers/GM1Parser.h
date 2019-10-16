/*
 * GM1Parser
 * Loads gm1 animation from file
 * Allan Legemaate
 * 12/12/2018
 */

#ifndef GM1PARSER_H
#define GM1PARSER_H

#include <allegro.h>
#include <string>
#include <vector>

struct GM1Data {
  unsigned int index = -1;
  unsigned int size = 0;
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int offset = 0;
  BITMAP *image = nullptr;
};

class GM1Parser {
  public:
    // Ctor
    GM1Parser() {};

    // Dtor
    virtual ~GM1Parser() {};

    // Load tgx from file
    static std::vector<BITMAP*> load_gm1(char const *filename);

  private:
    // Token to string converter
    static std::string data_type_name(int data_type);

    // Convert 4 chars to int32
    static unsigned int chars_to_int(char a, char b, char c, char d);

    // Load animation
    static BITMAP* load_gm1_animation(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data, std::vector<unsigned int> *pall);

    // Load tile
    static BITMAP* load_gm1_tile(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data);

    // Load font
    static BITMAP* load_gm1_tgx(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data);

    // Load uncompressed
    static BITMAP* load_gm1_uncompressed(std::vector<char> *bytes, unsigned int *iter, GM1Data *image_data);

};

#endif // GM1PARSER_H
