#include "keyListener.h"

bool keyListener::keyPressed[KEY_MAX] = { false};
bool keyListener::keyReleased[KEY_MAX] = { false};
bool keyListener::lastTicksKey[KEY_MAX] = { false};

// Check those keys!
void keyListener::update() {
  // Check key just pressed
  for (int i = 0; i < KEY_MAX; i++) {
    // Clear old values
    keyPressed[i] = false;
    keyReleased[i] = false;

    // Pressed since last tick?
    if ((bool)key[i] == true && lastTicksKey[i] == false) {
      keyPressed[i] = true;
    }

    // Released since last tick?
    if ((bool)key[i] == false && lastTicksKey[i] == true) {
      keyReleased[i] = true;
    }
  }


  // Get new values
  for (int i = 0; i < KEY_MAX; i++){
    // Key changed
    if (lastTicksKey[i] != (bool)key[i]) {
        lastTicksKey[i] = key[i];
    }
  }
}
