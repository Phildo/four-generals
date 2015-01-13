#ifndef _FG_EVENT_H_
#define _FG_EVENT_H_

#include "turn.h"

static const char e_type_join_con    = 'a';
static const char e_type_leave_con   = 'b';

static const char e_type_assign_card = 'c';
static const char e_type_revoke_card = 'd';

static const char e_type_begin_play = 'e';
static const char e_type_commit_action = 'f';
static const char e_type_commit_actions = 'g';

static const char e_type_reset_game = 'h';

struct Event
{
  char type; //listed above

  union
  {
    struct
    {
      char connection;
    } join_con;
    struct
    {
      char connection;
    } leave_con;
    struct
    {
      char connection;
      char cardinal;
    } assign_card;
    struct
    {
      char connection;
      char cardinal;
    } revoke_card;
    struct
    {
      char null;
    } begin_play;
    struct
    {
      char connection;
      Turn turn;
    } commit_action;
    struct
    {
      char null;
    } commit_actions;
    struct
    {
      char null;
    } reset_game;
  };

  Event();
  Event(char *c);
  void zero();

  void serialize(char *c) const;
};

#endif

