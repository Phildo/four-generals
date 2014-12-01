#include "debug_list.h"
#include "logger.h"
#include "graphics.h"

DebugList *DebugList::sing;

DebugList::DebugList()
{
  last_known_i = 0;
}

DebugList *DebugList::inst()
{
  if(!sing) sing = new DebugList();
  return sing;
}

void DebugList::init(Graphics *g)
{
  list = UI::List(0,0,g->winWidth(), g->winHeight(),38);
}

void DebugList::tick()
{
  while(MemLog::inst()->logLen() > last_known_i)
  {
    list.append(MemLog::inst()->logAt(last_known_i));
    last_known_i++;
  }
}

void DebugList::draw(Graphics *g)
{
  list.draw(g);
}

