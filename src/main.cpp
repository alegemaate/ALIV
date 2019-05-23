/*
 * Allegro Image Viewer
 * Allan Legemaate
 * 30/12/2016
 * Made because Windows 10 sucks
 */

// Libs
#include <allegro.h>
#include "algif/algif.h"
#include "view.h"

// Close button handler (enables X button)
volatile int close_button_pressed = FALSE;

void close_button_handler(void) {
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Setup game
void setup() {
  // Load allegro library
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse();

  set_display_switch_mode(SWITCH_BACKGROUND);
  set_color_depth(32);

  // Close button
  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);

  // Set graphics mode
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 640, 0, 0);

  // Title
  set_window_title("ALIMG - Allegro Image Viewer");

  // Allow background
  set_display_switch_mode(SWITCH_BACKGROUND);
}

// Start here
int main (int argc, char *argv[]) {
  // Setup basic functionality
  setup();

  // Load images into view
  view* view_state = new view();

  for (int i = 1; i < argc; i++) {
    view_state -> LoadImage(argv[i]);
  }

  // Run loop
  while (!key[KEY_ESC] && !close_button_pressed) {
    view_state -> update();
    view_state -> draw();
  }

  // End
  return 0;
}
END_OF_MAIN();
