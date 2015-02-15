#ifndef _FG_DAY_PICKER_H_
#define _FG_DAY_PICKER_H_

#include "ui.h"

class Graphics;
struct In;

class DayPicker
{
  private:
    int lastdrag;

    UI::Box rect;
    UI::Button button;

    UI::Image sun;
    UI::Box dayRects[7];
    UI::Label dayLbls[7];
    int dayDist;
    void init();
  public:
    float day;
    float maxDay;

    DayPicker();
    DayPicker(UI::Box r);
    ~DayPicker();

    bool touch(In &in);
    void tick();
    void draw(Graphics *g);

    void setDay(float d);
    void setMaxDay(float d);
    bool dragging();
    void stopDragging();
};

#endif

