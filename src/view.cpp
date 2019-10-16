#include "view.h"

#include <loadpng.h>
#include <math.h>
#include <algorithm>

#include "loaders/JpegLoader.h"
#include "loaders/PngLoader.h"
#include "loaders/PcxLoader.h"
#include "loaders/TgaLoader.h"
#include "loaders/BmpLoader.h"
#include "loaders/GifLoader.h"
#include "loaders/TGXLoader.h"
#include "loaders/GM1Loader.h"

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
bool view::LoadImage(const char* location) {
  // Busy cursor
  select_mouse_cursor(MOUSE_CURSOR_BUSY);
  show_mouse(screen);

  // Get loader
  ImageLoader *loader = GetLoader(location);

  // Load
  if (loader) {
    loader -> Load(location);
    images.push_back(loader);

    select_mouse_cursor(MOUSE_CURSOR_ARROW);
    show_mouse(screen);

    return true;
  }

  return false;
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
      x -= float(mouse_x - (SCREEN_W/2 - x)) / 2;
      y -= float(mouse_y - (SCREEN_H/2 - y)) / 2;
    }
    else if (image_zoom < 100) {
      image_zoom *= 2;
      // Offset x and y
      x += float(mouse_x - (SCREEN_W/2 - x));
      y += float(mouse_y - (SCREEN_H/2 - y));
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
    if (images.at(image_index) -> GetHWRatio() < 1.0f) {
      // X
      if (x < SCREEN_W/2 * (1 - image_zoom))
        x = SCREEN_W/2 * (1 - image_zoom);
      else if (x > SCREEN_W/2 * (1 + image_zoom) - SCREEN_W)
        x = SCREEN_W/2 * (1 + image_zoom) - SCREEN_W;

      // Y
      if (y < SCREEN_H/2 * (1 - images.at(image_index) -> GetHWRatio() * image_zoom))
        y = SCREEN_H/2 * (1 - images.at(image_index) -> GetHWRatio() * image_zoom);
      else if (y > SCREEN_H/2 * (1 + image_zoom * images.at(image_index) -> GetHWRatio()) - SCREEN_H)
        y = SCREEN_H/2 * (1 + image_zoom * images.at(image_index) -> GetHWRatio()) - SCREEN_H;
    }
    else{
      // X
      if (x < SCREEN_W/2 * (1 - images.at(image_index) -> GetWHRatio() * image_zoom))
        x = SCREEN_W/2 * (1 - images.at(image_index) -> GetWHRatio() * image_zoom);
      else if (x > SCREEN_W/2 * (1 + images.at(image_index) -> GetWHRatio() * image_zoom) - SCREEN_W)
        x = SCREEN_W/2 * (1 + images.at(image_index) -> GetWHRatio() * image_zoom) - SCREEN_W;

      // Y
      if (y < SCREEN_H/2 * (1 - image_zoom))
        y = SCREEN_H/2 * (1 - image_zoom);
      else if (y > SCREEN_H/2 * (1 + image_zoom) - SCREEN_H)
        y = SCREEN_H/2 * (1 + image_zoom) - SCREEN_H;
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
    std::string titlePath = std::string("ALIMG - ") + images.at(image_index) -> GetLocation();
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
    if (images.at(image_index) -> GetBitmap() == nullptr) {
      textprintf_centre_ex(buffer, font, SCREEN_W/2, SCREEN_H/2, 0xFFFFFF, -1, "Oh Noes! Could not load image..."/*images.at(image_index).errorMessage.c_str()*/);
    }
    else if (images.at(image_index) -> GetHWRatio() < 1.0f) {
      stretch_sprite(buffer, images.at(image_index) -> GetBitmap(),
                     SCREEN_W/2 * (1 - image_zoom) - x,
                     SCREEN_H/2 * (1 - images.at(image_index) -> GetHWRatio() * image_zoom) - y,
                     SCREEN_W * image_zoom,
                     SCREEN_H * image_zoom * images.at(image_index) -> GetHWRatio());
    }
    else {
      stretch_sprite(buffer, images.at(image_index) -> GetBitmap(),
                     SCREEN_W/2 * (1 - images.at(image_index) -> GetWHRatio() * image_zoom) - x,
                     SCREEN_H/2 * (1 - image_zoom) - y,
                     SCREEN_W * image_zoom * images.at(image_index) -> GetWHRatio(),
                     SCREEN_H * image_zoom);
    }
  }
  // No images
  else {
    textprintf_centre_ex(buffer, font, SCREEN_W/2, SCREEN_H/2, 0xFFFFFF, -1, "No image!");
  }

  // Image index
  textprintf_ex(buffer, font, 4, 4, 0xFFFFFF, -1, "image: %i/%i", image_index + (images.size() > 0 ? 1 : 0), images.size());

  //textprintf_ex( buffer, font, 0, 0, 0xFFFFFF, -1, "%i, %i", x, y);
  //textprintf_ex( buffer, font, 0, 20, 0xFFFFFF, -1, "%i, %i", SCREEN_W/2 - x, SCREEN_H/2 - y);
  //textprintf_ex( buffer, font, 0, 40, 0xFFFFFF, -1, "%f", image_zoom);

  // Draw buffer to screen
  draw_sprite(screen, buffer, 0, 0);
}

// Get image type
ImageLoader* view::GetLoader(const char* path) {
  // Extract type
  std::string sPath = path;
  std::string type = sPath.substr(sPath.find_last_of(".") + 1);
  std::transform(type.begin(), type.end(), type.begin(), ::tolower);

  // JPG
  if (type == "jpg" || type == "jpeg" || type == "jpe" || type == "jif" || type == "jfif" || type == "jfi") {
    return new JpegLoader();
  }
  // PNG
  else if (type == "png") {
    return new PngLoader();
  }
  // PCX
  else if (type == "pcx") {
    return new PcxLoader();
  }
  // BMP
  else if (type == "bmp" || type == "dib") {
    return new BmpLoader();
  }
  // GIF
  else if (type == "gif") {
    return new GifLoader();
  }
  // TGA
  else if (type == "tga" || type == "icb" || type == "vda" || type == "vst") {
    return new TgaLoader();
  }
  // TGX
  else if (type == "tgx") {
    return new TgxLoader();
  }
  // GM1
  else if (type == "gm1") {
    return new Gm1Loader();
  }

  // File type not supported
  return nullptr;
}
