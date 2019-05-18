#include "init.h"

// Construct state
init::init(){
  // Set graphics mode
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 640, 0, 0);

  // Title
  set_window_title("ALIMG - Allegro Image Viewer");
}

// Update
void init::update(){
  set_next_state(STATE_VIEW);
}
