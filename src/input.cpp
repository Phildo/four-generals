#include "input.h"
#include "graphics.h"
#include "logger.h"

void In::zero()
{
  x = 0;
  y = 0;
  type = NONE;
}

Input::Input(Graphics *g)
{
  graphics = g;
}

bool Input::poll(In &in)
{
  in.zero();
  if(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT) in.type = QUIT;
    #ifdef FG_PC
    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      in.type = In::DOWN;
      in.x = event.button.x-graphics->offsetX();
      in.y = event.button.y-graphics->offsetY();
    }
    else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
      in.type = In::UP;
      in.x = event.button.x-graphics->offsetX();
      in.y = event.button.y-graphics->offsetY();
    }
    else if(event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK))
    {
      in.type = In::MOVE;
      in.x = event.motion.x-graphics->offsetX();
      in.y = event.motion.y-graphics->offsetY();
    }
    #elif defined FG_ANDROID
    else if(event.type == SDL_FINGERDOWN)
    {
      in.type = In::DOWN;
      in.x = (event.tfinger.x*graphics->trueWinWidth())-graphics->offsetX();
      in.y = (event.tfinger.y*graphics->trueWinHeight())-graphics->offsetY();
    }
    else if(event.type == SDL_FINGERUP)
    {
      in.type = In::UP;
      in.x = (event.tfinger.x*graphics->trueWinWidth())-graphics->offsetX();
      in.y = (event.tfinger.y*graphics->trueWinHeight())-graphics->offsetY();
    }
    else if(event.type == SDL_FINGERMOTION)
    {
      in.type = In::MOVE;
      in.x = (event.tfinger.x*graphics->trueWinWidth())-graphics->offsetX();
      in.y = (event.tfinger.y*graphics->trueWinHeight())-graphics->offsetY();
    }
    #endif
    #ifdef FG_HALF_SIZE
    in.x *= 2;
    in.y *= 2;
    #endif
    return true;
  }
  return false;
}

Input::~Input()
{

}

