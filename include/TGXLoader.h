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

class TGXLoader {
  public:
    // Ctor
    TGXLoader() {};

    // Dtor
    virtual ~TGXLoader() {};

    // Load tgx from file
    static BITMAP* load_tgx(char const *filename, PALETTE pal);

  private:
    // Token to string converter
    static std::string token_name(int token);

    // Convert 16 bit to 24 colour
    static int convert_color(unsigned char byte1, unsigned char byte2);
};

#endif // TGXLOADER_H
