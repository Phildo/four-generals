#include "messager.h"
#include "graphics.h"
#include "compass.h"

Messager::Messager() : box(100,100,100,100)
{
  who  = UI::Image(Sprite::gen_n_0, box.x + space(box.w,20,30,2,0), box.y+20, 30, 30);
  when = UI::Image(Sprite::sun,     box.x + space(box.w,20,30,2,1), box.y+20, 30, 30);
}

Messager::~Messager()
{
}

void Messager::tick()
{

}

void Messager::draw(Graphics *g)
{
  box.draw(g);
  who.draw(g);
  when.draw(g);
}

void Messager::setMessage(Action m)
{
  message = m;
  who.sprite = Sprite::generals[Compass::icardinal(m.who)];
  when.sprite = Sprite::sun;
}

