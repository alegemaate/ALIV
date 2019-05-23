/*
 * Game state
 * Allan Legemaate
 * 30/12/2016
 * Displays the image
 */

#ifndef VIEW_H
#define VIEW_H

#include <allegro.h>
#include <vector>
#include <string>

#include "ImageLoader.h"
#include "image_data.h"
#include "state.h"
#include "keyListener.h"

#define WINDOW_W SCREEN_W
#define WINDOW_H SCREEN_H

enum {
  TYPE_INV,
  TYPE_PNG,
  TYPE_BMP,
  TYPE_PCX,
  TYPE_JPG,
  TYPE_GIF,
  TYPE_TGA,
  TYPE_TGX,
  TYPE_GM1
};

class view : public state{
  public:
    // Construct/deconstruct
    view();
    virtual ~view() {};

    // Override parent
    virtual void update() override;

    // Draw view
    virtual void draw() override;

    // Load image from file
    bool LoadImage(const char* location);
  protected:

  private:
    // Gets loader for file
    ImageLoader* GetLoader(const char* location);

    // Listens to keys for down and up
    keyListener the_listener;

    // Image buffer
    BITMAP *buffer;

    // Zoom factor
    float image_zoom;

    // Index
    int image_index;

    // Remember scroll
    int old_scroll;

    // Offset x and y
    int x, y;

    // Click and drag position
    int click_x, click_y;
    bool dragging;

    // Animate ticker
    int animate_ticker = 0;

    // All images
    std::vector<ImageLoader*> images;
};

#endif // GAME_H

