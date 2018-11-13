/*
 * Image Data
 * Allan Legemaate
 * 30/12/2016
 * Stores all image data needed to view
 */

#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <allegro.h>


class image_data{
  public:
    image_data(BITMAP *image);
    virtual ~image_data() {};

    // Data we collect
    BITMAP *image;

    int width;
    int height;

    bool wide;

    float wh_ratio;
    float hw_ratio;

  protected:

  private:
};

#endif // IMAGE_DATA_H
