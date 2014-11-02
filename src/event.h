#ifndef _FG_EVENT_H
#define _FG_EVENT_H

#include "network.h"

namespace Network
{
  static const int e_ser_len = 10;

  static const char e_type_ack         = 'a'; //handled entirely at network level (never reaches models)

  static const char e_type_assign_con  = 'b'; //server->client only
  static const char e_type_revoke_con  = 'c'; //server->client only
  static const char e_type_refuse_con  = 'd'; //server->client only

  static const char e_type_join_con    = 'e';
  static const char e_type_leave_con   = 'f';

  static const char e_type_assign_card = 'g';
  static const char e_type_revoke_card = 'h';

  static const char e_type_begin_play = 'i';

  struct Event //all members chars for quick/simple serializability
  {
    char connection;
    char cardinal;
    char type;
    char id_c[FG_EVT_MAX_DEC_LEN]; //string val of id_i (ie "2415")
    const char null;
    int id_i;

    //default constructor
    Event();

    //copy constructor
    Event(const Event &e);
    Event &operator=(const Event &e);

    //test for equality is a lie for comparing events to acks and retrieving from circ_q
    bool operator==(const Event& e){ return connection == e.connection; }
    bool operator!=(const Event& e){ return !operator==(e); }

    //custom constructor
    Event(char con, char card, char t, int id);

    //serializability
    char *serialize();
    Event(char *c);
  };
}

#endif

