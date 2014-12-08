#ifndef _FG_TWEEN_RECT_H_
#define _FG_TWEEN_RECT_H_

#include "tween.h"
#include "SDL.h"

class TweenRect
{
  private:
  public:
    Tween x;
    Tween y;
    Tween w;
    Tween h;

    TweenRect();
    TweenRect(SDL_Rect s, SDL_Rect e);
    TweenRect(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh);
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

