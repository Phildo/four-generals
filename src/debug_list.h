#ifndef _FG_DEBUG_LIST_H_
#define _FG_DEBUG_LIST_H_

#include "ui.h"

class Graphics;

class DebugList;
class DebugList
{
  private:
    static DebugList *sing;
    UI::List list;
    DebugList();
    int last_known_i;
  public:
    static DebugList *inst();
    void init(Graphics *g);
    void tick();
    void draw(Graphics *g);
};

#endif

