#include "action.h"

Action::Action() //set to pseudo 'null'
{
  zero();
};

Action::Action(char *c) //inverse of serialize
{
  int i = 0;
  what  = c[i]; i++;
  which = c[i]; i++;
  who   = c[i]; i++;
  when  = c[i]; i++;
  route = c[i]; i++;
}

int Action::power()
{
  if(what == 'a' || what == 'd') return 2;
  else if(what == '0') return 0;
  else return 1;
}

void Action::zero()
{
  what  = '0';
  which = '0';
  who   = '0';
  when  = '0';
  route = '0';
}

void Action::serialize(char *c) const
{
  int i = 0;
  c[i] = what;  i++;
  c[i] = which; i++;
  c[i] = who;   i++;
  c[i] = when;  i++;
  c[i] = route; i++;
}

void Action::beSabotaged(Action a)
{
  if(a.what == 's')
  {
    if(a.which == 'o') who  = a.who;
    if(a.which == 'e') when = a.when;
  }
}

