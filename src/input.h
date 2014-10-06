#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL.h>

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
    int winWidth;
    int winHeight;

    SDL_Event event;
  public:
    Input(Graphics *g);
    ~Input();
    bool poll(In &in, bool &put, bool &quit);
};

#endif

