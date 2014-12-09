#include "lerp_rect.h"

LerpRect::LerpRect() : x(0.0f,0.0f), y(0.0f,0.0f), w(0.0f,0.0f), h(0.0f,0.0f) {}
LerpRect::LerpRect(SDL_Rect s, SDL_Rect e) : x(s.x,e.x), y(s.y,e.y), w(s.w,e.w), h(s.h,e.h) { }
LerpRect::LerpRect(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh) : x(sx,ex), y(sy,ey), w(sw,ew), h(sh,eh) { }

SDL_Rect LerpRect::v()
{
  SDL_Rect r;
  r.x = x.v();
  r.y = y.v();
  r.w = w.v();
  r.h = h.v();
  return r;
}

SDL_Rect LerpRect::v(float _t)
{
  SDL_Rect r;
  r.x = x.v(_t);
  r.y = y.v(_t);
  r.w = w.v(_t);
  r.h = h.v(_t);
  return r;
}

void LerpRect::set(SDL_Rect s, SDL_Rect e)
{
  x.set(s.x,e.x);
  y.set(s.y,e.y);
  w.set(s.w,e.w);
  h.set(s.h,e.h);
}

void LerpRect::set(SDL_Rect s, SDL_Rect e, float t)
{
  x.set(s.x,e.x,t);
  y.set(s.y,e.y,t);
  w.set(s.w,e.w,t);
  h.set(s.h,e.h,t);
}

void LerpRect::set(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh)
{
  x.set(sx,ex);
  y.set(sy,ey);
  w.set(sw,ew);
  h.set(sh,eh);
}

void LerpRect::set(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh, float t)
{
  x.set(sx,ex,t);
  y.set(sy,ey,t);
  w.set(sw,ew,t);
  h.set(sh,eh,t);
}

void LerpRect::tick(float d)
{
  x.tick(d);
  y.tick(d);
  w.tick(d);
  h.tick(d);
}

void LerpRect::reset()
{
  x.reset();
  y.reset();
  w.reset();
  h.reset();
}

SDL_Rect LerpRect::lerp(SDL_Rect s, SDL_Rect e, float t)
{
  SDL_Rect tmp;
  tmp.x = Lerp::lerp(s.x,e.x,t);
  tmp.y = Lerp::lerp(s.y,e.y,t);
  tmp.w = Lerp::lerp(s.w,e.w,t);
  tmp.h = Lerp::lerp(s.h,e.h,t);
  return tmp;
}

SDL_Rect LerpRect::lerp(float sx, float sy, float sw, float sh, float ex, float ey, float ew, float eh, float t)
{
  SDL_Rect tmp;
  tmp.x = Lerp::lerp(sx,ex,t);
  tmp.y = Lerp::lerp(sy,ey,t);
  tmp.w = Lerp::lerp(sw,ew,t);
  tmp.h = Lerp::lerp(sh,eh,t);
  return tmp;
}

