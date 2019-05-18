#ifndef JPEGLOADER_H
#define JPEGLOADER_H

#include <stdlib.h>
#include <stdio.h>

#if defined(__MINGW32__)
  #undef __MINGW32__
  #include <jpeglib.h>
  #define __MINGW32__
#else
  #include <jpeglib.h>
#endif

#include <jerror.h>
#include <setjmp.h>

#include <allegro.h>


typedef struct my_src_mgr my_src_mgr;
struct my_src_mgr {
  struct jpeg_source_mgr pub;
  JOCTET eoi_buffer[2];
};

typedef struct my_error_mgr *my_error_ptr;
struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

char lastError[JMSG_LENGTH_MAX];

BITMAP* load_jpg(char const *filename, PALETTE pal);

#endif // JPEGLOADER_H
