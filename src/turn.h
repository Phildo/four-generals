#ifndef _FG_TURN_H_
#define _FG_TURN_H_

#include "action.h"

struct Turn
{
  char cardinal; //'n|e|s|w'
  Action actions[2];

  Turn();
  Turn(char *c);

  int power();
  Action *action(char what);

  void zero();

  void serialize(char *c) const;
};

#endif

