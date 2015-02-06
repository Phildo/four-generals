#include "messager.h"
#include "graphics.h"
#include "compass.h"

Messager::Messager() {}
Messager::Messager(int x, int y, int w, int h) : box(x,y,w,h)
{
  who  = UI::Image(Sprite::gen_n_0, box.x + space(box.w,20,100,2,0), box.y+20, 100, 100);
  when = UI::Image(Sprite::sun,     box.x + space(box.w,20,100,2,1), box.y+20, 100, 100);
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

