#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL.h>

namespace Sprite
{
  SDL_Rect tl_border();
  SDL_Rect t_border();
  SDL_Rect tr_border();
  SDL_Rect l_border();
  SDL_Rect r_border();
  SDL_Rect bl_border();
  SDL_Rect b_border();
  SDL_Rect br_border();
};

#endif

