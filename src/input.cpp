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
      in.x = event.button.x;
      in.y = event.button.y;
    }
    #elif defined FG_ANDROID
    else if(event.type == SDL_FINGERDOWN)
    {
      put = true;
      in.x = event.tfinger.x*winWidth;
      in.y = event.tfinger.y*winHeight;
    }
    #endif
    return true;
  }
  return false;
}

Input::~Input()
{

}

