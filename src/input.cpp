#include "input.h"
#include "graphics.h"

Input::Input(Graphics *g)
{
  graphics = g;
  winWidth = g->winWidth();
  winHeight = g->winHeight();
}

bool Input::poll(In &in, bool &quit)
{
  quit = false;
  if(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT) quit = true;
    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      in.x = event.button.x;
      in.y = event.button.y;
    }
    else if(event.type == SDL_FINGERDOWN)
    {
      in.x = event.tfinger.x*winWidth;
      in.y = event.tfinger.y*winHeight;
    }
    return true;
  }
  return false;
}

Input::~Input()
{

}

