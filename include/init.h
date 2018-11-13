/*
 * Init state
 * Allan Legemaate
 * 30/12/2016
 * Program inits here
 */

#ifndef INIT_H
#define INIT_H

#include <allegro.h>

#include "state.h"

class init : public state{
  public:
    // Construct/deconstruct
    init();
    ~init() {};

    // Override parent
    void update();
    void draw() {};

  protected:

  private:
};

#endif // INIT_H
