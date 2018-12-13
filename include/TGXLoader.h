/*
 * TGXLoader
 * Loads tgx image from file
 * Allan Legemaate
 * 12/12/2018
 */

#ifndef TGXLOADER_H
#define TGXLOADER_H

#include <allegro.h>
#include <string>
#include <vector>

class TGXLoader {
  public:
    // Ctor
    TGXLoader() {};

    // Dtor
    virtual ~TGXLoader() {};

    // Load tgx from file
    static BITMAP* load_tgx(char const *filename, PALETTE pal);

    // Tgx helper used by file and memory
    static BITMAP* load_tgx_helper(std::vector<char> *bytes, unsigned int *iter, unsigned int width, unsigned int height, std::vector<unsigned int>* pall = nullptr);

    // Convert 15 bit to 24 colour
    static int convert_color(unsigned char byte1, unsigned char byte2);

  private:
    // Token to string converter
    static std::string token_name(int token);
};

#endif // TGXLOADER_H
