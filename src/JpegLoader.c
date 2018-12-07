#include "JpegLoader.h"

void my_error_exit (j_common_ptr cinfo) {
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Clear error buffer */
  memset(lastError, 0, sizeof lastError);

  /* Save error message */
  (*cinfo->err->format_message) (cinfo, lastError);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

BITMAP* load_jpg_helper(FILE *f, unsigned char *memory, int size) {
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;

  /* Step 1: allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return NULL;
  }

  /* Step 2: specify data source (eg, a file) */
  jpeg_create_decompress(&cinfo);

  if (f) {
    jpeg_stdio_src(&cinfo, f);
  }

  /* Step 3: read file parameters with jpeg_read_header() */
  (void) jpeg_read_header(&cinfo, TRUE);

  /* Step 5: Start decompressor */
  (void) jpeg_start_decompress(&cinfo);

  int w = cinfo.output_width;
  int h = cinfo.output_height;
  int s = cinfo.output_components;

  BITMAP *bmp = create_bitmap_ex(24, w, h);
  unsigned char *buffer[1];
  unsigned char temp[w * s];
  buffer[0] = temp;
  unsigned char *p = bmp->dat;

  /* Step 6: Populate allegro bitmap */
  while (cinfo.output_scanline < h) {
    int j = cinfo.output_scanline;
    (void) jpeg_read_scanlines(&cinfo, (void *)buffer, 1);
    /* Grayscale */
    if (s == 1) {
      for (int i = 0; i < w; i++) {
        unsigned char c = buffer[0][i];
        p[j * w * 3 + i * 3 + 0] = c;
        p[j * w * 3 + i * 3 + 1] = c;
        p[j * w * 3 + i * 3 + 2] = c;
      }
    }
    /* RGB */
    else if (s == 3) {
      for (int i = 0; i < w; i++) {
        unsigned char r = buffer[0][i * s + 2];
        unsigned char g = buffer[0][i * s + 1];
        unsigned char b = buffer[0][i * s + 0];
        p[j * w * 3 + i * 3 + 0] = r;
        p[j * w * 3 + i * 3 + 1] = g;
        p[j * w * 3 + i * 3 + 2] = b;
      }
    }
    else {
      printf("Error! Cannot read JPEG data.\n");
      return NULL;
    }
  }

  /* Step 7: Finish decompression */
  (void) jpeg_finish_decompress(&cinfo);

  /* Step 8: Release JPEG decompression object */
  jpeg_destroy_decompress(&cinfo);

  /* Step 9: Return allegro bmp */
  return bmp;
}

BITMAP* load_jpg(char const *filename, PALETTE pal) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    return NULL;
  }
  BITMAP *bmp = load_jpg_helper(f, NULL, 0);
  fclose(f);
  return bmp;
}
