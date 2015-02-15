#ifndef _FG_MESSAGER_H_
#define _FG_MESSAGER_H_

#include "action.h"
#include "ui.h"

class Graphics;
struct In;

class Messager
{
  private:
    Action message;
    Action sabotage;

    UI::Box box;

    UI::Image who_m;
    UI::Image when_m;
    UI::Label when_l_m;

    UI::Image who_m_x;
    UI::Image when_m_x;

    UI::Image who_s;
    UI::Image when_s;
    UI::Label when_l_s;
  public:
    Messager();
    Messager(int x, int y, int w, int h);
    ~Messager();

    void tick();
    void draw(Graphics *g);

    void setMessage(Action m);
    void setMessageSabotage(Action m, Action s);
};

#endif

