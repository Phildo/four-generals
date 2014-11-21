#include "input.h"
#include "graphics.h"
#include "logger.h"

Input::Input(Graphics *g)
{
  graphics = g;
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
      in.x = (event.button.x-graphics->offsetX())*2;
      in.y = (event.button.y-graphics->offsetY())*2;
      #else
      in.x = event.button.x-graphics->offsetX();
      in.y = event.button.y-graphics->offsetY();
      #endif
    }
    #elif defined FG_ANDROID
    else if(event.type == SDL_FINGERDOWN)
    {
      put = true;
      #ifdef FG_HALF_SIZE
      in.x = ((event.tfinger.x*graphics->trueWinWidth())-graphics->offsetX())*2;
      in.y = ((event.tfinger.y*graphics->trueWinHeight())-graphics->offsetY())*2;
      #else
      in.x = (event.tfinger.x*graphics->trueWinWidth())-graphics->offsetX();
      in.y = (event.tfinger.y*graphics->trueWinHeight())-graphics->offsetY();
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

