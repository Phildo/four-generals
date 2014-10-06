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
    void draw(Graphics *g);
    Button();
    Button(SDL_Rect r);
    Button(int x, int y, int w, int h);
  };

  struct String
  {
    SDL_Rect rect; //really only uses its x,y
    char *string;
    int length;
    static const int w = 18;
    static const int h = 31;
    void draw(Graphics *g);
    String();
    String(SDL_Rect r, char *s, int l);
    String(int x, int y, char *s, int l);
  };
}

#endif

