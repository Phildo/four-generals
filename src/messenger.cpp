#include "messenger.h"

#include <cstring> //memset for simple initialization

Messenger::Messenger()
{
  memset(this, '0', sizeof(Messenger));
}

Messenger::Messenger(Event e)
: from(e.cardinal), at(e.cardinal), to(e.to), what(e.what), who(e.who), when(e.when), where(e.where)
{
}

Messenger::Messenger(char fro, char a, char t, char wat, char wo, char wen, char were)
: from(fro), at(a), to(t), what(wat), who(wo), when(wen), where(were)
{
}

bool Messenger::advance()
{
  if(at != to)
  {
    if(at != where) at = where;
    else at = to;
    return true;
  }
  return false;
}

Messenger::~Messenger()
{
}

