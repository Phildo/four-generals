#include "messager.h"
#include "graphics.h"
#include "compass.h"

Messager::Messager() {}
Messager::Messager(int x, int y, int w, int h) : box(x,y,w,h)
{
  who    = UI::Image(Sprite::gen_n_0, box.x + space(box.w,20,100,2,0), box.y+20, 100, 100);
  when   = UI::Image(Sprite::sun,     box.x + space(box.w,20,100,2,1), box.y+20, 100, 100);
  when_l = UI::Label("Sun",           box.x + space(box.w,20,100,2,1), box.y+20, 50);
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
  when_l.draw(g);
}

void Messager::setMessage(Action m)
{
  message = m;
  who.sprite = Sprite::generals[Compass::icardinal(m.who)];
  switch(m.when)
  {
    case 's': when_l.text = "Sun"; break;
    case 'm': when_l.text = "Mon"; break;
    case 't': when_l.text = "Tue"; break;
    case 'w': when_l.text = "Wed"; break;
    case 'h': when_l.text = "Thu"; break;
    case 'f': when_l.text = "Fri"; break;
    case 'a': when_l.text = "Sat"; break;
    default : break;
  }
}

