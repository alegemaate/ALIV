/*
 * Game state
 * Allan Legemaate
 * 30/12/2016
 * Displays the image
 */

#ifndef GAME_H
#define GAME_H

#include <allegro.h>
#include <vector>
#include <math.h>
#include <iostream>

#include "image_data.h"
#include "globals.h"
#include "state.h"
#include "keyListener.h"

#define WINDOW_W SCREEN_W
#define WINDOW_H SCREEN_H

class game : public state{
  public:
    // Construct/deconstruct
    game();
    ~game();

    // Override parent
    void update();
    void draw();
  protected:

  private:
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

    // All images
    std::vector<image_data> images;
};

#endif // GAME_H

