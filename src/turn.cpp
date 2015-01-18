#include "turn.h"

Turn::Turn() //set to pseudo 'null'
{
  zero();
};
Turn::Turn(char *c) //inverse of serialize
{
  int i = 0;
  cardinal = c[i]; i++;
  actions[0] = Action(&c[i]); i += sizeof(Action); //a bit sketch...
  actions[1] = Action(&c[i]); i += sizeof(Action); //a bit sketch...
}

void Turn::zero()
{
  cardinal = '0';
  actions[0].zero();
  actions[1].zero();
}

void Turn::serialize(char *c) const
{
  int i = 0;
  c[i] = cardinal; i++;
  actions[0].serialize(&c[i]); i += sizeof(Action); //a bit sketch...
  actions[1].serialize(&c[i]); i += sizeof(Action); //a bit sketch...
}

