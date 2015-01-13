#ifndef _FG_ACTION_H_
#define _FG_ACTION_H_

#include "defines.h"

struct Action //all members chars for quick/simple serializability
{
  //anything w/ value '0' = unassigned / N/A
  char what;  //'a' = attack, 'd' = defend, 'm' = messenger, 's' = sabotage, 'c' = scout
  char how;   //'b' = block, 'r' = read, 's' = switch //corresponds to sabotage members
  char which; //'o' = who, 'e' = when                 //corresponds to sabotage members
  char who;   //'n|e|s|w'       //corresponds to attack/messenger/sabotage members
  char when;  //'s|m|t|w|h|f|a' //corresponds to messenger/sabotage members
  char route; //'n|e|s|w'       //corresponds to messenger members

  Action();
  Action(char *c);
  void zero();

  void serialize(char *c) const;

  void beSabotaged(Action a); //mutates self dictated by a
};

#endif

