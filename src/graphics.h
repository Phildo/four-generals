#ifndef _FG_GRAPHICS_H_
#define _FG_GRAPHICS_H_

#include "SDL.h"
#include "SDL_image.h"

class Graphics
{
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* tex;

    int xshake;
    int yshake;
  public:
    Graphics();
    ~Graphics();

    void draw(const SDL_Rect& src, const SDL_Rect& dest);
    void drawInMask(SDL_Rect src, SDL_Rect dest, SDL_Rect mask);
    void drawAt(const SDL_Rect& src, int x, int y);
    void clear();
    void flip();

    void setShake(int x, int y);

    int winHeight();
    int winWidth();
    int trueWinWidth();
    int trueWinHeight();
    int offsetX();
    int offsetY();
    SDL_Rect mapRect(SDL_Rect in);
    int texHeight();
    int texWidth();
};

#endif

