#ifndef TGXLOADER_H
#define TGXLOADER_H

#include <allegro.h>
#include <string>

class TGXLoader {
  public:
    TGXLoader();
    virtual ~TGXLoader();

    static BITMAP* load_tgx(char const *filename, PALETTE pal);

  protected:

  private:
    static std::string token_name(int token);
    static int convert_color(unsigned char byte1, unsigned char byte2);
};

#endif // TGXLOADER_H
