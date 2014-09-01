#ifndef _UNITS_H_
#define _UNITS_H_

#include <SDL.h>
#include "defines.h"

//types (probably redundant)
typedef int screen_p; //1
typedef int sprite_p; //4
typedef int block_p; //80

namespace Units
{
  //ratios
  #ifndef FG_HALF_SIZE
  const int screen_r = 1; //size of screen pixel, in pixels
  const int sprite_r = 4; //size of sprite pixel, in screen pixels
  const int block_r = 20; //size of sprite block, in sprite pixels
  #else
  const int screen_r = 1;
  const int sprite_r = 2;
  const int block_r = 20;
  #endif

  screen_p spriteToScreen(sprite_p p);
  SDL_Rect spriteToScreen(sprite_p x, sprite_p y, sprite_p w, sprite_p h);
  SDL_Rect spriteToScreen(SDL_Rect p);

  block_p spriteToBlock(sprite_p p);
  SDL_Rect spriteToBlock(sprite_p x, sprite_p y, sprite_p w, sprite_p h);
  SDL_Rect spriteToBlock(SDL_Rect p);

  sprite_p screenToSprite(screen_p p);
  SDL_Rect screenToSprite(screen_p x, screen_p y, screen_p w, screen_p h);
  SDL_Rect screenToSprite(SDL_Rect p);

  block_p screenToBlock(screen_p p);
  SDL_Rect screenToBlock(block_p x, block_p y, block_p w, block_p h);
  SDL_Rect screenToBlock(SDL_Rect p);

  screen_p blockToScreen(block_p p);
  SDL_Rect blockToScreen(block_p x, block_p y, block_p w, block_p h);
  SDL_Rect blockToScreen(SDL_Rect p);

  sprite_p blockToSprite(block_p p);
  SDL_Rect blockToSprite(block_p x, block_p y, block_p w, block_p h);
  SDL_Rect blockToSprite(SDL_Rect p);
};

#endif

