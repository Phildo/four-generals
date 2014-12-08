#ifndef _FG_INPUT_H_
#define _FG_INPUT_H_

#include "SDL.h"

class Graphics;

struct In
{
  int x;
  int y;
};

class Input
{
  private:
    Graphics *graphics;

    SDL_Event event;
  public:
    Input(Graphics *g);
    ~Input();
    bool poll(In &in, bool &put, bool &quit);
};

#endif

