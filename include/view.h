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
#include "keyListener.h"

class view {
  public:
    // Construct/deconstruct
    view();
    ~view() {};

    // Override parent
    void update();

    // Draw view
    void draw();

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

