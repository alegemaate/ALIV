/*
 * GM1Loader
 * Loads gm1 animation from file
 * Allan Legemaate
 * 12/12/2018
 */

#ifndef GM1LOADER_H
#define GM1LOADER_H

#include <allegro.h>
#include <string>

class GM1Loader {
  public:
    // Ctor
    GM1Loader() {};

    // Dtor
    virtual ~GM1Loader() {};

    // Load tgx from file
    static BITMAP* load_gm1(char const *filename, PALETTE pal);

  private:
    // Token to string converter
    static std::string data_type_name(int data_type);
};

#endif // GM1LOADER_H
