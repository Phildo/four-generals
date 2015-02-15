#include "day_picker.h"
#include "graphics.h"
#include "input.h"

static float clamp(float v, float min, float max)
{
  if(v < min) return min;
  if(v > max) return max;
  return v;
}
static float snapToInt(float v)
{
  float left = (float)((int)v);
  float right = (float)((int)(v+1.0f));
  if(v < left +0.01f) return left;
  if(v > right-0.01f) return right;
  return v;
}

DayPicker::DayPicker() : rect(0,0,100,40)
{
  init();
}

DayPicker::DayPicker(UI::Box r) : rect(r)
{
  init();
}

void DayPicker::init()
{
  lastdrag = -1;
  day = 0.0;
  maxDay = 0.0;

  button.rect = rect;
  sun = UI::Image(Sprite::sun, rect.x+rect.w/2-rect.h/2, rect.y, rect.h, rect.h);

  char daynamehacks[] = {'S','u','\0','M','o','\0','T','u','\0','W','e','\0','T','h','\0','F','r','\0','S','a','\0'};
  for(int i = 0; i < 7; i++)
  {
    int offset = (i+3)%7; //leftmost day is Th (center is Su)
    dayRects[i] = UI::Box(space(rect.w,0,rect.h,7,offset), rect.y, rect.h, rect.h);
    dayLbls[i] = UI::Label(&daynamehacks[i*3], dayRects[i].rect);
  }
  dayDist = dayRects[1].x-dayRects[0].x;
  setDay(0.0f);
}

DayPicker::~DayPicker()
{
}

bool DayPicker::touch(In &in)
{
  if(in.type == In::UP) lastdrag = -1;
  if(in.type == In::DOWN && button.query(in)) lastdrag = in.x;
  if(in.type == In::MOVE && lastdrag != -1)
  {
    float delta = in.x-lastdrag;
    setDay(day-(delta/150.0f));
    lastdrag = in.x;
  }

  return lastdrag != -1;
}

void DayPicker::tick()
{
}

void DayPicker::draw(Graphics *g)
{
  sun.draw(g);
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(g);
}

void DayPicker::setDay(float d)
{
  day = clamp(snapToInt(d), 0.0f, maxDay);
  for(int i = 0; i < 7; i++)
    dayLbls[i].rect.x = (((dayRects[i].rect.x-dayRects[4].rect.x-(int)(dayDist*day))+(100*7*dayDist))%(7*dayDist))+dayRects[4].rect.x;
}

void DayPicker::setMaxDay(float d)
{
  maxDay = d;
}

bool DayPicker::dragging()
{
  return lastdrag != -1;
}

void DayPicker::stopDragging()
{
  lastdrag = -1;
}

