/*
 * Allegro Image Viewer
 * Allan Legemaate
 * 30/12/2016
 * Made because Windows 10 sucks
 */

// Libs
#include <allegro.h>

// Extentions to support PNG, JPG and GIF
// Allegro handles BMP, TIF and a few others
#include <alpng.h>
#include <jpgalleg.h>

#include "algif/algif.h"
#include "state.h"
#include "init.h"
#include "view.h"

// Current state object
state *currentState = NULL;

// Delete game state and free state resources
void clean_up() {
  delete currentState;
}

// Close button handler (enables X button)
volatile int close_button_pressed = FALSE;

void close_button_handler(void) {
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Change game state
void change_state() {
  //If the state needs to be changed
  if(nextState != STATE_NULL) {
    //Delete the current state
    if(nextState != STATE_EXIT) {
      delete currentState;
    }

    //Change the state
    switch(nextState) {
      case STATE_INIT:
        currentState = new init();
        break;
      case STATE_VIEW :
        currentState = new view();
        break;
      default:
        currentState = new init();
        break;
    }

    //Change the current state ID
    stateID = nextState;

    //NULL the next state ID
    nextState = STATE_NULL;
  }
}

// Setup game
void setup() {
  // Load allegro library
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse();
  install_joystick(JOY_TYPE_AUTODETECT);
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, ".");
  set_color_depth(32);

  // Addons
  alpng_init();
  jpgalleg_init();
  algif_init();

  // Close button
  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);
}

// Start here
int main (int argc, char *argv[]) {
  // Setup basic functionality
  setup();

  //Set the current state ID
  set_next_state(STATE_INIT);
  change_state();
  set_next_state(STATE_VIEW);
  change_state();

  // Load images into view
  view* view_state = dynamic_cast<view*>(currentState);

  // Ensure view_state casted and load images
  if (view_state != nullptr) {
    for (int i = 1; i < argc; i++) {
      view_state -> load_image(std::string(argv[i]));
    }
  }

  // Run loop
  while (!key[KEY_ESC] && !close_button_pressed) {
    currentState -> update();
    currentState -> draw();
    change_state();
  }

  //Clean up
  clean_up();

  // End
  return 0;
}
END_OF_MAIN();
