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
    UI::Box box;
    UI::Image who;
    UI::Image when;
  public:
    Messager();
    ~Messager();

    void tick();
    void draw(Graphics *g);

    void setMessage(Action m);
};

#endif

