#ifndef _FG_LERP_RECT_H_
#define _FG_LERP_RECT_H_

#include "lerp.h"
#include "SDL.h"

class LerpRect
{
  private:
  public:
    Lerp x;
    Lerp y;
    Lerp w;
    Lerp h;

    LerpRect();
    LerpRect(SDL_Rect s, SDL_Rect e);
    LerpRect(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh);
    SDL_Rect v();
    SDL_Rect v(float _t);
    void set(SDL_Rect s, SDL_Rect e);
    void set(SDL_Rect s, SDL_Rect e, float t);
    void set(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh);
    void set(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh, float t);
    void tick(float d);
    void reset();
};

#endif

