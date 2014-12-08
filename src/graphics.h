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
  public:
    Graphics();
    ~Graphics();

    void draw(const SDL_Rect& src, const SDL_Rect& dest);
    void drawAt(const SDL_Rect& src, int x, int y);
    void clear();
    void flip();

    int winHeight();
    int winWidth();
    int trueWinWidth();
    int trueWinHeight();
    int offsetX();
    int offsetY();
    int texHeight();
    int texWidth();
};

#endif

