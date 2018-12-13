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
  TYPE_TGX
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
    bool load_image(std::string location);
  protected:

  private:
    // Gets file type
    int image_type(std::string location);

    // Converts string to lowercase
    std::string strLower(std::string str);

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

