#include "graphics.h"
#include "defines.h"
#include "logger.h"

#define FG_FAKE_WIDTH 960
#define FG_FAKE_HEIGHT 540

Graphics::Graphics()
{
  #ifdef FG_ANDROID
  window = SDL_CreateWindow(NULL, 0, 0, 0, 0, 0);
  #elif defined FG_PC
  window = SDL_CreateWindow(NULL, 0, 0, FG_FAKE_WIDTH, FG_FAKE_HEIGHT, 0);
  #endif

  renderer = SDL_CreateRenderer(window, -1, 0);

  //clear color
  SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);

  //load tex
  #ifdef FG_ANDROID
  // load sample.png into image
  SDL_Surface *surf = IMG_Load("tex.png");
  #elif defined FG_PC
  SDL_Surface *surf = IMG_Load("../assets/tex.png");
  #endif

  tex = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);

  xshake = 0;
  yshake = 0;
}

Graphics::~Graphics()
{
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Graphics::draw(const SDL_Rect& src, const SDL_Rect& dest)
{
  SDL_Rect offsetDest = dest;
  offsetDest.x += xshake;
  offsetDest.y += yshake;
  offsetDest = mapRect(offsetDest);
  SDL_RenderCopy(renderer, tex, &src, &offsetDest);
}

void Graphics::drawInMask(SDL_Rect src, SDL_Rect dest, SDL_Rect mask)
{
  if(dest.x < mask.x)
  {
    int d = mask.x-dest.x;
    float f = (float)d/((float)dest.w);
    int cut = (src.w*f);
    src.x += cut;
    src.w -= cut;
    dest.x += d;
    dest.w -= d;
  }
  if(dest.y < mask.y)
  {
    int d = mask.y-dest.y;
    float f = (float)d/((float)dest.h);
    int cut = (src.h*f);
    src.y += cut;
    src.h -= cut;
    dest.y += d;
    dest.h -= d;
  }
  if(dest.x+dest.w > mask.x+mask.w)
  {
    int d = (dest.x+dest.w)-(mask.x+mask.w);
    float f = (float)d/((float)dest.w);
    int cut = (src.w*f);
    src.w -= cut;
    dest.w -= d;
  }
  if(dest.y+dest.h > mask.y+mask.h)
  {
    int d = (dest.y+dest.h)-(mask.y+mask.h);
    float f = (float)d/((float)dest.h);
    int cut = (src.h*f);
    src.h -= cut;
    dest.h -= d;
  }

  SDL_Rect offsetDest = dest;
  offsetDest.x += xshake;
  offsetDest.y += yshake;
  offsetDest = mapRect(offsetDest);
  SDL_RenderCopy(renderer, tex, &src, &offsetDest);
}

void Graphics::drawAt(const SDL_Rect& src, int x, int y)
{
  SDL_Rect offsetDest;
  offsetDest.x = x+xshake;
  offsetDest.y = y+yshake;
  offsetDest.w = src.w;
  offsetDest.h = src.h;
  offsetDest = mapRect(offsetDest);
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

void Graphics::setShake(int x, int y)
{
  xshake = x;
  yshake = y;
}

int Graphics::winWidth()  { return FG_FAKE_WIDTH; }
int Graphics::winHeight() { return FG_FAKE_HEIGHT; }
int Graphics::trueWinWidth()  { int w; SDL_GetWindowSize(window, &w, NULL); return w; }
int Graphics::trueWinHeight() { int h; SDL_GetWindowSize(window, NULL, &h); return h; }
int Graphics::offsetX() { return (trueWinWidth()-winWidth())/2; }
int Graphics::offsetY() { return (trueWinHeight()-winHeight())/2; }
SDL_Rect Graphics::mapRect(SDL_Rect in)
{
  SDL_Rect r;
  if(offsetX() >= 0 && offsetY() >= 0)
  {
    r.x = in.x+offsetX();
    r.y = in.y+offsetY();
    r.w = in.w;
    r.h = in.h;
  }
  else
  {
    r.x = ((float)in.x/(float)winWidth())*trueWinWidth();
    r.y = ((float)in.y/(float)winHeight())*trueWinHeight();
    r.w = ((float)in.w/(float)winWidth())*trueWinWidth();
    r.h = ((float)in.h/(float)winHeight())*trueWinHeight();
  }
  return r;
}
int Graphics::texWidth()  { int w; SDL_QueryTexture(tex, NULL, NULL, &w, NULL); return w; }
int Graphics::texHeight() { int h; SDL_QueryTexture(tex, NULL, NULL, NULL, &h); return h; }

