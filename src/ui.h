#ifndef _UI_H_
#define _UI_H_

#include <SDL.h>
struct In;
class Graphics;

namespace UI
{
  struct Button
  {
    SDL_Rect rect;
    bool query(const In &in);
    virtual void draw(Graphics *g);
    Button();
    Button(SDL_Rect r);
    Button(int x, int y, int w, int h);
  };
}

#endif

