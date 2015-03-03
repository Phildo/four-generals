#ifndef _FG_INPUT_H_
#define _FG_INPUT_H_

#include "SDL.h"

class Graphics;

struct In
{
  enum IN_TYPE
  {
    NONE,
    DOWN,
    UP,
    MOVE,
    QUIT,
    COUNT
  } type;
  int x;
  int y;
  void zero();
};

class Input
{
  private:
    Graphics *graphics;

    SDL_Event event;
  public:
    Input(Graphics *g);
    ~Input();
    bool poll(In &in);
    In mapIn(In in);
};

#endif

