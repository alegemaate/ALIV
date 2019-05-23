#include "view.h"

#include <loadpng.h>
#include <math.h>
#include <iostream>

#include "JpegLoader.h"
#include "algif/algif.h"

#include "TGXLoader.h"
#include "GM1Loader.h"

// Constrct
view::view() {
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
  // Busy cursor
  select_mouse_cursor(MOUSE_CURSOR_BUSY);
  show_mouse(screen);

  // Temp bitmap
  BITMAP *tempBitmap = NULL;

  // Error message
  std::string errorMessage = "";

  // Image type
  int imageType = image_type(location);

  // Load JPG
  if (imageType == TYPE_PNG) {
    tempBitmap = load_png(location.c_str(), NULL);
  }
  // Png
  else if (imageType == TYPE_JPG) {
    JpegLoader *jLoad = new JpegLoader();
    jLoad -> Load(location.c_str());

    tempBitmap = jLoad -> GetBitmap();
    //errorMessage = lastError;
  }
  // Gif
  else if (imageType == TYPE_GIF) {
    BITMAP **frames = nullptr;
    int *durations = nullptr;
    unsigned int number_frams = algif_load_animation (location.c_str(), &frames, &durations);

    // Parse gif
    for (unsigned int i = 0; i < number_frams; i ++) {
      images.push_back(image_data(frames[i], location, errorMessage));
    }

    // Ready cursor
    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    show_mouse(screen);

    return true;
  }
  // Other (allegro supported)
  else if (imageType == TYPE_BMP ||
           imageType == TYPE_PCX ||
           imageType == TYPE_TGA) {
    tempBitmap = load_bitmap(location.c_str(), NULL);
  }
  // SHC Tgx format
  else if (imageType == TYPE_TGX) {
    tempBitmap = TGXLoader::load_tgx(location.c_str(), NULL);
  }
  // SHC GM1 format
  else if (imageType == TYPE_GM1) {
    std::vector<BITMAP*> return_bitmaps = GM1Loader::load_gm1(location.c_str(), NULL);

    // Load all animation images
    for (unsigned int i = 0; i < return_bitmaps.size(); i++) {
      images.push_back(image_data(return_bitmaps.at(i), location, errorMessage));
    }
    // Ready cursor
    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    show_mouse(screen);

    return true;
  }


  // Make an image data type
  image_data tempImageData = image_data(tempBitmap, location, errorMessage);
  images.push_back(tempImageData);

  // Ready cursor
  select_mouse_cursor(MOUSE_CURSOR_ARROW);
  show_mouse(screen);

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
  if (image_zoom > 1.0f && images.size() > 0) {
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

  // Select image
  if (keyListener::keyPressed[KEY_LEFT] && images.size() != 0)
    image_index--;
  if (keyListener::keyPressed[KEY_RIGHT] && images.size() != 0)
    image_index++;

  // "Animate"
  if (key[KEY_A]) {
    animate_ticker ++;
    if (animate_ticker > 20) {
      image_index ++;
      animate_ticker = 0;
    }
  }

  // Looperoni
  if (image_index < 0)
    image_index = images.size() - 1;

  else if ((unsigned)image_index >= images.size())
    image_index = 0;

  // Set title
  if (images.size() > 0) {
    std::string titlePath = "ALIMG - " + images.at(image_index).filePath;
    set_window_title(titlePath.c_str());
  }
}

// Draw
void view::draw(){
  // Clear buffer to color
  clear_to_color( buffer, 0x000000);

  // Draw all images stretched if needed
  if (images.size() > (unsigned)image_index){
    // Unloadable image
    if (images.at(image_index).image == NULL) {
      textprintf_centre_ex(buffer, font, WINDOW_W/2, WINDOW_H/2, 0xFFFFFF, -1, images.at(image_index).errorMessage.c_str());
    }
    else if (images.at(image_index).wide) {
      stretch_sprite(buffer, images.at(image_index).image,
                     WINDOW_W/2 * (1 - image_zoom) - x,
                     WINDOW_H/2 * (1 - images.at(image_index).hw_ratio * image_zoom) - y,
                     WINDOW_W * image_zoom,
                     WINDOW_H * image_zoom * images.at(image_index).hw_ratio);
    }
    else {
      stretch_sprite(buffer, images.at(image_index).image,
                     WINDOW_W/2 * (1 - images.at(image_index).wh_ratio * image_zoom) - x,
                     WINDOW_H/2 * (1 - image_zoom) - y,
                     WINDOW_W * image_zoom * images.at(image_index).wh_ratio,
                     WINDOW_H * image_zoom);
    }
  }
  // No images
  else {
    textprintf_centre_ex(buffer, font, WINDOW_W/2, WINDOW_H/2, 0xFFFFFF, -1, "No image!");
  }

  // Image index
  textprintf_ex(buffer, font, 4, 4, 0xFFFFFF, -1, "image: %i/%i", image_index, images.size());

  //textprintf_ex( buffer, font, 0, 0, 0xFFFFFF, -1, "%i, %i", x, y);
  //textprintf_ex( buffer, font, 0, 20, 0xFFFFFF, -1, "%i, %i", WINDOW_W/2 - x, WINDOW_H/2 - y);
  //textprintf_ex( buffer, font, 0, 40, 0xFFFFFF, -1, "%f", image_zoom);

  // Draw buffer to screen
  draw_sprite(screen, buffer, 0, 0);
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
  // TGX
  else if (type == "tgx") {
    return TYPE_TGX;
  }
  // GM1
  else if (type == "gm1") {
    return TYPE_GM1;
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
