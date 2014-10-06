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

  tmp = Sprite::border_tl();
  tmp.x = rect.x;
  tmp.y = rect.y;
  g->draw(Sprite::border_tl(),tmp);

  tmp = Sprite::border_t();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.w = rect.w;
  g->draw(Sprite::border_t(), tmp);

  tmp = Sprite::border_tr();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  g->draw(Sprite::border_tr(),tmp);

  tmp = Sprite::border_r();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_r(), tmp);

  tmp = Sprite::border_br();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::border_br(),tmp);

  tmp = Sprite::border_b();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  tmp.w = rect.w;
  g->draw(Sprite::border_b(), tmp);

  tmp = Sprite::border_bl();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::border_bl(),tmp);

  tmp = Sprite::border_l();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_l(), tmp);
}

String::String() { }
String::String(SDL_Rect r, char *s, int l)
{
  rect = r;
  r.w = w;
  r.h = h;
  string = s;
  length = l;
}
String::String(int x, int y, char *s, int l)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  string = s;
  length = l;
}

void String::draw(Graphics *g)
{
  SDL_Rect tmp = rect;
  for(int i = 0; i < length; i++)
  {
    g->draw(Sprite::alpha(*(string+i)),tmp);
    tmp.x += w;
  }
}

