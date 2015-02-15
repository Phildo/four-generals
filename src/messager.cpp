#include "messager.h"
#include "graphics.h"
#include "compass.h"

Messager::Messager() {}
Messager::Messager(int x, int y, int w, int h) : box(x,y,w,h)
{
  who_m    = UI::Image(Sprite::gen_n_0, box.x + space(box.w,20,100,2,0), box.y+20, 100, 100);
  when_m   = UI::Image(Sprite::sun,     box.x + space(box.w,20,100,2,1), box.y+20, 100, 100);
  when_l_m = UI::Label("Sun",           box.x + space(box.w,20,100,2,1), box.y+20, 50);

  who_m_x  = UI::Image(Sprite::ex,      box.x + space(box.w,20,100,2,0), box.y+20, 100, 100);
  when_m_x = UI::Image(Sprite::ex,      box.x + space(box.w,20,100,2,1), box.y+20, 100, 100);

  who_s    = UI::Image(Sprite::gen_n_0, box.x + space(box.w,20,100,2,0), box.y+140, 100, 100);
  when_s   = UI::Image(Sprite::sun,     box.x + space(box.w,20,100,2,1), box.y+140, 100, 100);
  when_l_s = UI::Label("Sun",           box.x + space(box.w,20,100,2,1), box.y+140, 50);

  message.zero();
  sabotage.zero();
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

  who_m.draw(g);
  when_m.draw(g);
  when_l_m.draw(g);

  if(sabotage.what == 's')
  {
    if(sabotage.which == 'o') who_m_x.draw(g);
    if(sabotage.which == 'e') when_m_x.draw(g);

    who_s.draw(g);
    when_s.draw(g);
    when_l_s.draw(g);
  }
}

void Messager::setMessage(Action m)
{
  message = m;
  who_m.sprite = Sprite::generals[Compass::icardinal(m.who)];
  switch(m.when)
  {
    case 's': when_l_m.text = "Sun"; break;
    case 'm': when_l_m.text = "Mon"; break;
    case 't': when_l_m.text = "Tue"; break;
    case 'w': when_l_m.text = "Wed"; break;
    case 'h': when_l_m.text = "Thu"; break;
    case 'f': when_l_m.text = "Fri"; break;
    case 'a': when_l_m.text = "Sat"; break;
    default : break;
  }
  sabotage.zero();
}

void Messager::setMessageSabotage(Action m, Action s)
{
  setMessage(m);
  sabotage = s;

  who_s.sprite = who_m.sprite;
  when_l_s.text = when_l_m.text;

  if(sabotage.which == 'o') who_s.sprite = Sprite::generals[Compass::icardinal(s.who)];
  if(sabotage.which == 'e')
  {
    switch(s.when)
    {
      case 's': when_l_s.text = "Sun"; break;
      case 'm': when_l_s.text = "Mon"; break;
      case 't': when_l_s.text = "Tue"; break;
      case 'w': when_l_s.text = "Wed"; break;
      case 'h': when_l_s.text = "Thu"; break;
      case 'f': when_l_s.text = "Fri"; break;
      case 'a': when_l_s.text = "Sat"; break;
      default : break;
    }
  }
}

