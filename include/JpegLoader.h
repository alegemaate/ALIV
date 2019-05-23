#ifndef JPEGLOADER_H
#define JPEGLOADER_H

#include <allegro.h>
#include "ImageLoader.h"

extern "C" {
  #if defined(__MINGW32__)
    #undef __MINGW32__
    #include <jpeglib.h>
    #define __MINGW32__
  #else
    #include <jpeglib.h>
  #endif

  #include <jerror.h>
  #include <setjmp.h>
}

class JpegLoader: public ImageLoader {
  public:
    virtual int Load(const char* filename) override;

  private:
    virtual int LoadBitmap() override;

    uint8_t nNumComponent;

    struct ErrorManager {
      jpeg_error_mgr defaultErrorManager;
      jmp_buf jumpBuffer;
    };

    static void ErrorExit(j_common_ptr cinfo);
    static void OutputMessage(j_common_ptr cinfo);

};

#endif // JPEGLOADER_H
