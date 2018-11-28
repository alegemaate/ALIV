#include "view.h"

#include <alpng.h>
#include <jpgalleg.h>
#include <algif.h>

#include <math.h>

#include <iostream>

// Constrct
view::view(){
  // Loading cursor
  enable_hardware_cursor();
  select_mouse_cursor(MOUSE_CURSOR_BUSY);
  show_mouse(screen);

  // Create buffer
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  // Zoom percent
  image_zoom = 1.0f;

  // Index
  image_index = 0;

  // Scroll at current scroll
  old_scroll = mouse_z;

  // Image offset
  x = y = 0;

  // Click and drag
  click_x = click_y = -1;
  dragging = false;

  // Mouse
  select_mouse_cursor(MOUSE_CURSOR_ARROW);
  show_mouse(screen);
}

// Load image from path
bool view::load_image(std::string location) {
  // Temp bitmap
  BITMAP *tempBitmap;

  // Image type
  int imageType = image_type(location);

  // Load JPG
  if (imageType == TYPE_PNG) {
    tempBitmap = load_png(location.c_str(), NULL);
  }
  // Png
  else if (imageType == TYPE_JPG) {
    tempBitmap = load_jpg(location.c_str(), NULL);
  }
  // Gif
  else if (imageType == TYPE_GIF) {
    tempBitmap = load_gif(location.c_str(), NULL);
  }
  // Other (allegro supported)
  else if (imageType == TYPE_BMP ||
           imageType == TYPE_PCX ||
           imageType == TYPE_TGA) {
    tempBitmap = load_bitmap(location.c_str(), NULL);
  }
  // Unknown type
  else {
    return false;
  }

  // Could not load
  if (!tempBitmap)
    return false;

  // Make an image data type
  image_data tempImageData = image_data(tempBitmap);
  images.push_back(tempImageData);
  return true;
}

// Update
void view::update() {
  // Check keys
  the_listener.update();

  // Image zooming
  if (old_scroll != mouse_z){
    if (mouse_z - old_scroll < 0 && image_zoom > 0.01) {
      image_zoom /= 2;
      // Offset x and y
      x -= float(mouse_x - (WINDOW_W/2 - x)) / 2;
      y -= float(mouse_y - (WINDOW_H/2 - y)) / 2;
    }
    else if (image_zoom < 100) {
      image_zoom *= 2;
      // Offset x and y
      x += float(mouse_x - (WINDOW_W/2 - x));
      y += float(mouse_y - (WINDOW_H/2 - y));
    }
    old_scroll = mouse_z;
  }



  // Move image around
  if (mouse_b & 1) {
    if (dragging) {
      x += click_x - mouse_x;
      y += click_y - mouse_y;
    }
    click_x = mouse_x;
    click_y = mouse_y;
    dragging = true;
  }
  else {
    click_x = click_y = -1;
    dragging = false;
  }

  // Prevent image from going off screen
  if (image_zoom > 1.0f) {
    // Different corners
    if (images.at(image_index).wide) {
      // X
      if (x < WINDOW_W/2 * (1 - image_zoom))
        x = WINDOW_W/2 * (1 - image_zoom);
      else if (x > WINDOW_W/2 * (1 + image_zoom) - WINDOW_W)
        x = WINDOW_W/2 * (1 + image_zoom) - WINDOW_W;

      // Y
      if (y < WINDOW_H/2 * (1 - images.at(image_index).hw_ratio * image_zoom))
        y = WINDOW_H/2 * (1 - images.at(image_index).hw_ratio * image_zoom);
      else if (y > WINDOW_H/2 * (1 + image_zoom * images.at(image_index).hw_ratio) - WINDOW_H)
        y = WINDOW_H/2 * (1 + image_zoom * images.at(image_index).hw_ratio) - WINDOW_H;
    }
    else{
      // X
      if (x < WINDOW_W/2 * (1 - images.at(image_index).wh_ratio * image_zoom))
        x = WINDOW_W/2 * (1 - images.at(image_index).wh_ratio * image_zoom);
      else if (x > WINDOW_W/2 * (1 + images.at(image_index).wh_ratio * image_zoom) - WINDOW_W)
        x = WINDOW_W/2 * (1 + images.at(image_index).wh_ratio * image_zoom) - WINDOW_W;

      // Y
      if (y < WINDOW_H/2 * (1 - image_zoom))
        y = WINDOW_H/2 * (1 - image_zoom);
      else if (y > WINDOW_H/2 * (1 + image_zoom) - WINDOW_H)
        y = WINDOW_H/2 * (1 + image_zoom) - WINDOW_H;
    }
  }

  if (image_zoom <= 1.0f){
    x = 0;
    y = 0;
  }

  // Move image around
  if (keyListener::keyPressed[KEY_LEFT])
    image_index--;
  if (keyListener::keyPressed[KEY_RIGHT])
    image_index++;

  // Looperoni
  if (image_index < 0)
    image_index = images.size() - 1;
  else if ((unsigned)image_index >= images.size())
    image_index = 0;
}

// Draw
void view::draw(){
  // Clear buffer to color
  clear_to_color( buffer, 0x000000);

  // Draw all images stretched if needed
  if (images.size() > (unsigned)image_index){
    if (images.at(image_index).wide) {
      //int h_center_location = WINDOW_H/2 * image_zoom * (1 - images.at(image_index).hw_ratio);
      stretch_sprite(buffer, images.at(image_index).image,
                     WINDOW_W/2 * (1 - image_zoom) - x,
                     WINDOW_H/2 * (1 - images.at(image_index).hw_ratio * image_zoom) - y,
                     WINDOW_W * image_zoom,
                     WINDOW_H * image_zoom * images.at(image_index).hw_ratio);
    }
    else {
      //int w_center_location = WINDOW_W/2 * image_zoom * (1 - images.at(image_index).wh_ratio);
      stretch_sprite(buffer, images.at(image_index).image,
                     WINDOW_W/2 * (1 - images.at(image_index).wh_ratio * image_zoom) - x,
                     WINDOW_H/2 * (1 - image_zoom) - y,
                     WINDOW_W * image_zoom * images.at(image_index).wh_ratio,
                     WINDOW_H * image_zoom);
    }
  }
  // No images
  else{
    textprintf_centre_ex( buffer, font, WINDOW_W/2, WINDOW_H/2, 0xFFFFFF, -1, "No image!");
  }

  //textprintf_ex( buffer, font, 0, 0, 0xFFFFFF, -1, "%i, %i", x, y);
  //textprintf_ex( buffer, font, 0, 20, 0xFFFFFF, -1, "%i, %i", WINDOW_W/2 - x, WINDOW_H/2 - y);
  //textprintf_ex( buffer, font, 0, 40, 0xFFFFFF, -1, "%f", image_zoom);

  // Draw buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}

// Get image type
int view::image_type(std::string path) {
  // Extract type
  std::string type = strLower(path.substr(path.find_last_of(".") + 1));

  // JPG
  if (type == "jpg" || type == "jpeg" || type == "jpe" || type == "jif" || type == "jfif" || type == "jfi") {
    return TYPE_JPG;
  }
  // PNG
  else if (type == "png") {
    return TYPE_PNG;
  }
  // PCX
  else if (type == "pcx") {
    return TYPE_PCX;
  }
  // BMP
  else if (type == "bmp" || type == "dib") {
    return TYPE_BMP;
  }
  // GIF
  else if (type == "gif") {
    return TYPE_GIF;
  }
  // TGA
  else if (type == "tga" || type == "icb" || type == "vda" || type == "vst") {
    return TYPE_TGA;
  }

  // File type not supported
  return TYPE_INV;
}

// Convert string to lower
std::string view::strLower(std::string str) {
  std::locale loc;
  std::string newStr = "";
  for (std::string::size_type i = 0; i < str.length(); ++i)
    newStr += std::tolower(str[i],loc);
  return newStr;
}
