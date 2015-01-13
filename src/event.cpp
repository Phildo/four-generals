#include "event.h"

Event::Event() //set to pseudo 'null'
{
  zero();
};
Event::Event(char *c) //MUST MANUALLY KEEP IN SYNC W/ SERIALIZE!
{
  int i = 0;
  type       = c[i]; i++;
  connection = c[i]; i++;

  switch(type)
  {
    case e_type_join_con:
      join_con.connection = c[i]; i++;
      break;
    case e_type_leave_con:
      leave_con.connection = c[i]; i++;
      break;
    case e_type_assign_card:
      assign_card.connection = c[i]; i++;
      assign_card.cardinal = c[i]; i++;
      break;
    case e_type_revoke_card:
      revoke_card.connection = c[i]; i++;
      revoke_card.cardinal = c[i]; i++;
      break;
    case e_type_begin_play:
      begin_play.null = c[i]; i++;
      break;
    case e_type_commit_action:
      commit_action.connection = c[i]; i++;
      commit_action.turn = Turn(&c[i]); i+= sizeof(Turn);
      break;
    case e_type_commit_actions:
      commit_actions.null = c[i]; i++;
      break;
    case e_type_reset_game:
      reset_game.null = c[i]; i++;
      break;
  }
}

void Event::zero()
{
  type = '0';
}

void Event::serialize(char *c) const
{
  int i = 0;
  c[i] = type; i++;

  switch(type)
  {
    case e_type_join_con:
      c[i] = join_con.connection; i++;
      break;
    case e_type_leave_con:
      c[i] = leave_con.connection; i++;
      break;
    case e_type_assign_card:
      c[i] = assign_card.connection; i++;
      c[i] = assign_card.cardinal; i++;
      break;
    case e_type_revoke_card:
      c[i] = revoke_card.connection; i++;
      c[i] = revoke_card.cardinal; i++;
      break;
    case e_type_begin_play:
      c[i] = begin_play.null; i++;
      break;
    case e_type_commit_action:
      c[i] = commit_action.connection; i++;
      commit_action.turn.serialize(&c[i]); i+= sizeof(Turn);
      break;
    case e_type_commit_actions:
      c[i] = commit_actions.null; i++;
      break;
    case e_type_reset_game:
      c[i] = reset_game.null; i++;
      break;
  }
  c[i] = '\0'; //for good measure
}

