#ifndef _FG_EVENT_H
#define _FG_EVENT_H

#include "network.h"

namespace Network
{
  //to keep track of already assigned (CAPS = taken) (god what a terrible system)
  //AbCDefghiJkLmNopqRstuvwxyz
  static const char e_type_ack         = 'a';
  static const char e_type_assign_con  = 'c';
  static const char e_type_refuse_con  = 'n';
  static const char e_type_join        = 'j';
  static const char e_type_leave       = 'l';
  static const char e_type_assign_card = 'd';
  static const char e_type_revoke_card = 'r';
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
    int serlen();
  };
}

#endif

