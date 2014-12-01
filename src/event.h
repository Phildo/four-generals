#ifndef _FG_EVENT_H_
#define _FG_EVENT_H_

#include "defines.h"

static const char e_type_join_con    = 'e';
static const char e_type_leave_con   = 'f';

static const char e_type_assign_card = 'g';
static const char e_type_revoke_card = 'h';

static const char e_type_begin_play = 'i';
static const char e_type_commit_action = 'j';
static const char e_type_commit_actions = 'k';

static const char e_type_reset_game = 'l';

struct Event //all members chars for quick/simple serializability
{
  //anything w/ value '0' = unassigned / N/A
  char connection; //IDENTIFIER '1'-'5'
  char cardinal;   //IDENTIFIER 'n|e|s|w'
  char action;     //'a' = attack, 'd' = defend, 'm' = messenger, 's' = sabotage
  char how;        //corresponds to sabotage members
  char which;      //corresponds to sabotage members
  char to;         //cardinal (for whom the message is intended)
  char what;       //corresponds to messenger members
  char who;        //corresponds to messenger members
  char when;       //corresponds to messenger members
  char where;      //corresponds to messenger members
  char type;       //listed above
  int sabotage_id;
  int messenger_id;
  int evt_id;

  Event();
  Event(char *c);
  void zero();

  //serializability
  void serialize(char *c) const;

  char *humanAction();
  char h_action_buff[256];

  //test for equality is a lie for comparing events to acks and retrieving from circ_q
  bool operator==(const Event& e){ return connection == e.connection; }
  bool operator!=(const Event& e){ return !operator==(e); }

  //ONLY USE WHEN DEBUGGING (adds massive memory overhead)
  #ifdef FG_DEBUG //should cause errors when disabling debug
  void debug(char *buff);
  char event_type_buff[256];
  #endif
};

#endif

