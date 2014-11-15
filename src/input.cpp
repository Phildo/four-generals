#include "input.h"
#include "graphics.h"
#include "logger.h"

Input::Input(Graphics *g)
{
  graphics = g;
  winWidth = g->winWidth();
  winHeight = g->winHeight();
}

bool Input::poll(In &in, bool &put, bool &quit)
{
  quit = false;
  put = false;
  if(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT) quit = true;
    #ifdef FG_PC
    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      put = true;
      #ifdef FG_HALF_SIZE
      in.x = event.button.x*2;
      in.y = event.button.y*2;
      #else
      in.x = event.button.x;
      in.y = event.button.y;
      #endif
    }
    #elif defined FG_ANDROID
    else if(event.type == SDL_FINGERDOWN)
    {
      put = true;
      #ifdef FG_HALF_SIZE
      in.x = event.tfinger.x*winWidth*2;
      in.y = event.tfinger.y*winHeight*2;
      #else
      in.x = event.tfinger.x*winWidth;
      in.y = event.tfinger.y*winHeight;
      #endif
    }
    #endif
    return true;
  }
  return false;
}

Input::~Input()
{

}

