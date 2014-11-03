#ifndef _FG_EVENT_H
#define _FG_EVENT_H

#include "network.h"

namespace Network
{
  static const int e_ser_len = 9+FG_EVT_MAX_DEC_LEN+1; //oh god this is a terrible system

  static const char e_type_ack         = 'a'; //handled entirely at network level (never reaches models)

  static const char e_type_assign_con  = 'b'; //server->client only
  static const char e_type_revoke_con  = 'c'; //server->client only
  static const char e_type_refuse_con  = 'd'; //server->client only

  static const char e_type_join_con    = 'e';
  static const char e_type_leave_con   = 'f';

  static const char e_type_assign_card = 'g';
  static const char e_type_revoke_card = 'h';

  static const char e_type_begin_play = 'i';
  static const char e_type_commit_action = 'j';

  struct Event //all members chars for quick/simple serializability
  {
    //'0' = unassigned / N/A
    /* 1 */ char connection; //IDENTIFIER '1'-'5' //'5' = invalid
    /* 2 */ char cardinal;   //IDENTIFIER 'n|e|s|w'
    /* 3 */ char action;     //'a' = attack, 'd' = defend, 'm' = messenger, 's' = sabotage
    /* 4 */ char to;         //cardinal (for whom the message is intended)
    /* 5 */ char what;       //corresponds to messenger members
    /* 6 */ char who;        //corresponds to messenger members
    /* 7 */ char when;       //corresponds to messenger members
    /* 8 */ char where;      //corresponds to messenger members
    /* 9 */ char type;       //listed above
    /* 9+FG_EVT_MAX_DEC_LEN */ char id_c[FG_EVT_MAX_DEC_LEN]; //string val of id_i (ie "2415")
    /* 9+FG_EVT_MAX_DEC_LEN+1 */ char null; //not const because then we can't use default copy
    int id_i;

    Event();
    Event(char con, char card, char act, char t, char wat, char wo, char wen, char were, char ty, int id);
    Event(char *c);
    void zero();

    //test for equality is a lie for comparing events to acks and retrieving from circ_q
    bool operator==(const Event& e){ return connection == e.connection; }
    bool operator!=(const Event& e){ return !operator==(e); }

    //serializability
    char *serialize();

    //ONLY USE WHEN DEBUGGING (adds massive memory overhead)
    #ifdef FG_DEBUG //should cause errors when disabling debug
    char *human();
    char human_buff[256];
    char event_type_buff[256];
    #endif
  };
}

#endif

