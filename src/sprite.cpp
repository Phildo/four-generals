#include "sprite.h"

SDL_Rect rect(int x, int y, int w, int h) { SDL_Rect r; r.x = x; r.y = y; r.w = w; r.h = h; return r; }
SDL_Rect Sprite::tl_border() { return rect(100, 0,10,10); }
SDL_Rect Sprite::t_border()  { return rect(110, 0,10,10); }
SDL_Rect Sprite::tr_border() { return rect(120, 0,10,10); }
SDL_Rect Sprite::l_border()  { return rect(100,10,10,10); }
SDL_Rect Sprite::r_border()  { return rect(120,10,10,10); }
SDL_Rect Sprite::bl_border() { return rect(100,20,10,10); }
SDL_Rect Sprite::b_border()  { return rect(110,20,10,10); }
SDL_Rect Sprite::br_border() { return rect(120,20,10,10); }

