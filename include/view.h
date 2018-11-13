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
#include <math.h>
#include <iostream>
#include <string>

#include "image_data.h"
#include "state.h"
#include "keyListener.h"

#define WINDOW_W SCREEN_W
#define WINDOW_H SCREEN_H

class view : public state{
  public:
    // Construct/deconstruct
    view();
    ~view() {};

    // Override parent
    void update();
    void draw();
    void load_image(std::string location);
  protected:

  private:
    // Listens to keys for down and up
    keyListener the_listener;

    // Image buffer
    BITMAP *buffer;

    // Number of images loaded
    int image_num;

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

    // All images
    std::vector<image_data> images;
};

#endif // GAME_H

