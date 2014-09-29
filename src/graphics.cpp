#include "graphics.h"
#include "defines.h"

#ifndef FG_HALF_SIZE
#define FG_SAFE_WIDTH 1184
#define FG_SAFE_HEIGHT 720
#else
#define FG_SAFE_WIDTH 592
#define FG_SAFE_HEIGHT 360
#endif

Graphics::Graphics()
{
  #ifdef FG_ANDROID
  window = SDL_CreateWindow(NULL, 0, 0, 0, 0, 0);
  #elif defined FG_PC
  window = SDL_CreateWindow(NULL, 0, 0, FG_SAFE_WIDTH, FG_SAFE_HEIGHT, 0);
  #endif

  offsetX = (this->winWidth()-FG_SAFE_WIDTH)/2;
  offsetY = (this->winHeight()-FG_SAFE_HEIGHT)/2;

  renderer = SDL_CreateRenderer(window, -1, 0);

  //clear color
  SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);

  //load tex
  #ifdef FG_ANDROID
  SDL_Surface* surf = SDL_LoadBMP("tex.bmp");
  #elif defined FG_PC
  SDL_Surface* surf = SDL_LoadBMP("../assets/tex.bmp");
  #endif

  SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format,0xFF,0x00,0xFF));
  tex = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
}

Graphics::~Graphics()
{
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Graphics::draw(const SDL_Rect& src, const SDL_Rect& dest)
{
  SDL_Rect offsetDest;
  offsetDest.x = dest.x+offsetX;
  offsetDest.y = dest.y+offsetY;
  offsetDest.w = dest.w;
  offsetDest.h = dest.h;
  SDL_RenderCopy(renderer, tex, &src, &offsetDest);
}

void Graphics::drawAt(const SDL_Rect& src, int x, int y)
{
  SDL_Rect offsetDest;
  offsetDest.x = x+offsetX;
  offsetDest.y = y+offsetY;
  offsetDest.w = src.w;
  offsetDest.h = src.h;
  SDL_RenderCopy(renderer, tex, &src, &offsetDest);
}

void Graphics::clear()
{
  SDL_RenderClear(renderer);
}

void Graphics::flip()
{
  SDL_RenderPresent(renderer);
}

int Graphics::winWidth()  { int w; SDL_GetWindowSize(window, &w, NULL); return w; }
int Graphics::winHeight() { int h; SDL_GetWindowSize(window, NULL, &h); return h; }
int Graphics::texWidth()  { int w; SDL_QueryTexture(tex, NULL, NULL, &w, NULL); return w; }
int Graphics::texHeight() { int h; SDL_QueryTexture(tex, NULL, NULL, NULL, &h); return h; }

