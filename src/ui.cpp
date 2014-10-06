#include "ui.h"
#include "input.h"
#include "graphics.h"
#include "sprite.h"

using namespace UI;

Button::Button() { }
Button::Button(SDL_Rect r)
{
  rect = r;
}
Button::Button(int x, int y, int w, int h)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
}

bool Button::query(const In &in)
{
  return (in.x > rect.x && in.x < rect.x+rect.w &&
          in.y > rect.y && in.y < rect.y+rect.h);
}

void Button::draw(Graphics *g)
{
  SDL_Rect tmp;

  tmp = Sprite::tl_border();
  tmp.x = rect.x;
  tmp.y = rect.y;
  g->draw(Sprite::tl_border(),tmp);

  tmp = Sprite::t_border();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.w = rect.w;
  g->draw(Sprite::t_border(), tmp);

  tmp = Sprite::tr_border();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  g->draw(Sprite::tr_border(),tmp);

  tmp = Sprite::r_border();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::r_border(), tmp);

  tmp = Sprite::br_border();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::br_border(),tmp);

  tmp = Sprite::b_border();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  tmp.w = rect.w;
  g->draw(Sprite::b_border(), tmp);

  tmp = Sprite::bl_border();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::bl_border(),tmp);

  tmp = Sprite::l_border();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::l_border(), tmp);
}

