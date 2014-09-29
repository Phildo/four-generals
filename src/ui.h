#ifndef _UI_H_
#define _UI_H_

#include <SDL.h>

#include "graphics.h"

class UI
{
  private:
    Graphics *graphics;
  public:
    UI(Graphics *g);
    ~UI();
};

#endif

