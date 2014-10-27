#include "event.h"

#include "defines.h"
#include "logger.h"

#include "stdlib.h"

using namespace Network;

Event::Event()               : null('\0') {};
Event::Event(const Event &e) : null('\0')
{
  connection = e.connection;
  cardinal = e.cardinal;
  type = e.type;
  for(int i = 0; i < FG_EVT_MAX_DEC_LEN; i++)
    id_c[i] = e.id_c[i];
  id_i = e.id_i;
}
Event & Event::operator=(const Event &e)
{
  connection = e.connection;
  cardinal = e.cardinal;
  type = e.type;
  for(int i = 0; i < FG_EVT_MAX_DEC_LEN; i++)
    id_c[i] = e.id_c[i];
  id_i = e.id_i;
  return *this;
} //no need to set null
Event::Event(char con, char card, char t, int id) : connection(con), cardinal(card), type(t), id_c("00000"), null('\0'), id_i(id) { id_c[5] = '0'; /*would be '\0' fron initialiazer*/ }

char *Event::serialize()
{
  //fill id_c with 6 digit char rep of id (ie '000012')
  int tmp_left = id_i;
  int tmp_this = 0;
  for(int i = FG_EVT_MAX_DEC_LEN-1; i > 0; i--)
  {
    tmp_this = tmp_left%10;
    id_c[i] = '0'+tmp_this;
    tmp_left -= tmp_this; //prob don't need to do because of truncation in division
    tmp_left /= 10;
  }

  return (char *)&connection;
}
Event::Event(char *c) : null('\0')
{
  connection = c[0];
  cardinal = c[1];
  type = c[2];
  for(int i = 0; i < FG_EVT_MAX_DEC_LEN; i++)
    id_c[i] = c[3+i];
  id_i = atoi(id_c);
}

