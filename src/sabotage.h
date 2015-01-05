#ifndef _FG_SABOTAGE_H_
#define _FG_SABOTAGE_H_

#include "event.h"

class Sabotage
{
  private:
  public:
    char cardinal; //'owner' of sabotage
    char how;   //'b' = block, 'r' = read, 's' = sabotage
    char which; //'a' = what, 'o' = who, 'e' = when, 'r' = route
    char what;  //corresponds to messenger
    char who;   //corresponds to messenger
    char when;  //corresponds to messenger
    char route; //corresponds to messenger

    int id;

    Sabotage();
    Sabotage(Event e);
    Sabotage(char c, char ow, char wich, char wat, char wo, char wen, char were, int i);

    ~Sabotage();
};

#endif

