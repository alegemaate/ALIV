#include "JpegLoader.h"

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

// Load initial JPEG Data
int JpegLoader::Load(const char* filename) {
  // Data
  jpeg_decompress_struct cinfo;
  ErrorManager errorManager;

  // Load file
  FILE *pFile = fopen(filename, "rb");
  if (!pFile)
    return 1;

  // Allocate and initialize JPEG decompression object
  cinfo.err = jpeg_std_error(&errorManager.defaultErrorManager);
  errorManager.defaultErrorManager.error_exit = JpegLoader::ErrorExit;
  errorManager.defaultErrorManager.output_message = JpegLoader::OutputMessage;

  if (setjmp(errorManager.jumpBuffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(pFile);
    return 2;
  }

  // Populate cinfo
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, pFile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  // Extract header info to vars
  nWidth = cinfo.image_width;
  nHeight = cinfo.image_height;
  nNumComponent = cinfo.num_components;
  pData = new uint8_t[nWidth * nHeight * nNumComponent];

  // Fill in data
  while(cinfo.output_scanline < cinfo.image_height) {
		uint8_t* p = pData + cinfo.output_scanline * cinfo.image_width * cinfo.num_components;
		jpeg_read_scanlines(&cinfo, &pData, nHeight);
	}

  // Close
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);

	return LoadBitmap();
}

// Dump into Allegro BITMAP
int JpegLoader::LoadBitmap() {
  // Create allegro BITMAp of size
  pImage = create_bitmap(nWidth, nHeight);

  // Populate allegro bitmap
  for(uint32_t h = 0; h < nHeight; h++) {
    for(uint32_t w = 0; w < nWidth; w++) {

      // Grayscale
      if (nNumComponent == 1) {
        uint32_t index = (h * w + w);
        ((long *)pImage->line[h])[w] = makecol(pData[index], pData[index], pData[index]);
      }
      // RGB
      else if (nNumComponent == 3) {
        uint32_t index = (h * w + w) * 3;
        putpixel(pImage, w, h, makecol(pData[index + 0], pData[index + 1], pData[index + 2]));
        //((long *)pImage->line[h])[w] = makecol(pData[index + 2], pData[index + 1], pData[index]);
      }
      else {
        printf("Error! Cannot read JPEG data. Invalid number of components.\n");
        return 1;
      }
    }
  }

  return 0;
}

// JPEG error handler
void JpegLoader::ErrorExit(j_common_ptr cinfo) {
	ErrorManager* pErrorManager = (ErrorManager*) cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(pErrorManager->jumpBuffer, 1);
}

// JPEG message handler
void JpegLoader::OutputMessage(j_common_ptr cinfo) {
	// disable error messages
	/*char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
	fprintf(stderr, "%s\n", buffer);*/
}
