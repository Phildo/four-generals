#include "units.h"
#include "logger.h"

screen_p Units::spriteToScreen(sprite_p p) { return p*sprite_r; }
SDL_Rect Units::spriteToScreen(sprite_p x, sprite_p y, sprite_p w, sprite_p h) { SDL_Rect r; r.x = spriteToScreen(x); r.y = spriteToScreen(y); r.w = spriteToScreen(w); r.h = spriteToScreen(h); return r; }
SDL_Rect Units::spriteToScreen(SDL_Rect p) { return spriteToScreen(p.x, p.y, p.w, p.h); }

block_p Units::spriteToBlock(sprite_p p)  { return p/block_r; }
SDL_Rect Units::spriteToBlock(sprite_p x, sprite_p y, sprite_p w, sprite_p h) { SDL_Rect r; r.x = spriteToBlock(x); r.y = spriteToBlock(y); r.w = spriteToBlock(w); r.h = spriteToBlock(h); return r; }
SDL_Rect Units::spriteToBlock(SDL_Rect p) { return spriteToBlock(p.x, p.y, p.w, p.h); }

sprite_p Units::screenToSprite(screen_p p) { return p/sprite_r; }
SDL_Rect Units::screenToSprite(screen_p x, screen_p y, screen_p w, screen_p h) { SDL_Rect r; r.x = screenToSprite(x); r.y = screenToSprite(y); r.w = screenToSprite(w); r.h = screenToSprite(h); return r; }
SDL_Rect Units::screenToSprite(SDL_Rect p) { return screenToSprite(p.x, p.y, p.w, p.h); }

block_p Units::screenToBlock(screen_p p)  { return p/sprite_r/block_r; }
SDL_Rect Units::screenToBlock(block_p x, block_p y, block_p w, block_p h) { SDL_Rect r; r.x = screenToBlock(x); r.y = screenToBlock(y); r.w = screenToBlock(w); r.h = screenToBlock(h); return r; }
SDL_Rect Units::screenToBlock(SDL_Rect p) { return screenToBlock(p.x, p.y, p.w, p.h); }

screen_p Units::blockToScreen(block_p p)  { return p*block_r*sprite_r; }
SDL_Rect Units::blockToScreen(block_p x, block_p y, block_p w, block_p h) { SDL_Rect r; r.x = blockToScreen(x); r.y = blockToScreen(y); r.w = blockToScreen(w); r.h = blockToScreen(h); return r; }
SDL_Rect Units::blockToScreen(SDL_Rect p) { return blockToScreen(p.x, p.y, p.w, p.h); }

sprite_p Units::blockToSprite(block_p p)  { return p*block_r; }
SDL_Rect Units::blockToSprite(block_p x, block_p y, block_p w, block_p h) { SDL_Rect r; r.x = blockToSprite(x); r.y = blockToSprite(y); r.w = blockToSprite(w); r.h = blockToSprite(h); return r; }
SDL_Rect Units::blockToSprite(SDL_Rect p) { return blockToSprite(p.x, p.y, p.w, p.h); }

