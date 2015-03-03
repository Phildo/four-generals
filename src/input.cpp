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
    if(event.type == SDL_QUIT) in.type = In::QUIT;
    #ifdef FG_PC
    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      in.x = event.button.x;
      in.y = event.button.y;
      in = mapIn(in);
      in.type = In::DOWN;
    }
    else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
    {
      in.x = event.button.x;
      in.y = event.button.y;
      in = mapIn(in);
      in.type = In::UP;
    }
    else if(event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON_LMASK))
    {
      in.x = event.motion.x;
      in.y = event.motion.y;
      in = mapIn(in);
      in.type = In::MOVE;
    }
    #elif defined FG_ANDROID
    else if(event.type == SDL_FINGERDOWN)
    {
      in.x = event.tfinger.x*graphics->trueWinWidth();
      in.y = event.tfinger.y*graphics->trueWinHeight();
      in = mapIn(in);
      in.type = In::DOWN;
    }
    else if(event.type == SDL_FINGERUP)
    {
      in.x = event.tfinger.x*graphics->trueWinWidth();
      in.y = event.tfinger.y*graphics->trueWinHeight();
      in = mapIn(in);
      in.type = In::UP;
    }
    else if(event.type == SDL_FINGERMOTION)
    {
      in.x = event.tfinger.x*graphics->trueWinWidth();
      in.y = event.tfinger.y*graphics->trueWinHeight();
      in = mapIn(in);
      in.type = In::MOVE;
    }
    #endif

    return true;
  }
  return false;
}

In Input::mapIn(In in)
{
  In r;
  if(graphics->offsetX() >= 0 && graphics->offsetY() >= 0)
  {
    r.x = in.x-graphics->offsetX();
    r.y = in.y-graphics->offsetY();
  }
  else
  {
    r.x = ((float)in.x/(float)graphics->trueWinWidth())*graphics->winWidth();
    r.y = ((float)in.y/(float)graphics->trueWinHeight())*graphics->winHeight();
  }

  return r;
}

Input::~Input()
{

}

