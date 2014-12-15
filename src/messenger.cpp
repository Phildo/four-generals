#include "messenger.h"

#include "logger.h"

#include <stdio.h> //sprintf
#include <cstring> //memset for simple initialization

Messenger::Messenger()
{
  memset(this, '0', sizeof(Messenger));
  id = 0;
}

Messenger::Messenger(Event e)
: from(e.cardinal), was(e.cardinal), at(e.cardinal), to(e.to), what(e.what), who(e.who), when(e.when), where(e.where), id(e.messenger_id)
{
}

Messenger::Messenger(char fro, char a, char t, char wat, char wo, char wen, char were, int i)
: from(fro), was(a), at(a), to(t), what(wat), who(wo), when(wen), where(were), id(i)
{
}

bool Messenger::operator==(Messenger m)
{
  return m.id == id;
}

void Messenger::sabotage(Sabotage s)
{
  if(s.how == 's')
  {
    if(s.which == 'a') what  = s.what;
    if(s.which == 'o') who   = s.who;
    if(s.which == 'e') when  = s.when;
    if(s.which == 'r') where = s.where;
  }
}

char *Messenger::message()
{
  if(what == '0') sprintf(message_buff,"Incoming Message: ALAK SJVAWIA JLWKFJALWKDFJ X");
  if(what == 'a')
  {
    if(who == '0' && when == '0') sprintf(message_buff,"Incoming Message: Attack ASLDKFJI ALSEIFJ on LKAJ IOAJWLDKFJA!");
    else if(who == '0')           sprintf(message_buff,"Incoming Message: Attack JIJOJLKW DFKJ  on %c!",when);
    else if(when == '0')          sprintf(message_buff,"Incoming Message: Attack %c on AKJLIJ LAKWJF!",who);
    else                          sprintf(message_buff,"Incoming Message: Attack %c on %c!",who,when);
  }
  if(what == 'd')
  {
    if(when == '0') sprintf(message_buff,"Incoming Message: Defend on AKJLIJ LAKWJF!");
    else            sprintf(message_buff,"Incoming Message: Defend on %c!",when);
  }

  return &message_buff[0];
}

Messenger::~Messenger()
{
}

