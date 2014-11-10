#include "event.h"

#include "defines.h"
#include "logger.h"

#include "stdlib.h"
#include <cstring> //memcopy for simple serialization, memset for simple initialization

Event::Event() //set to pseudo 'null'
{
  zero();
};
Event::Event(char *c) //just memcopy that sucka
{
  memcpy(this, (void *)c, sizeof(Event));
  null = '\0';
  id_i = atoi(id_c);
}
Event::Event(char con, char card, char act, char t, char wat, char wo, char wen, char were, char ty, int id)
: connection(con), cardinal(card), action(act), to(t), what(wat), who(wo), when(wen), where(were), type(ty), id_c("00000"), id_i(id)
{
  id_c[5] = '0'; /*would be '\0' fron initialiazer*/
  null = '\0';
}

void Event::zero()
{
  memset(this, '0', sizeof(Event));
  null = '\0';
  id_i = 0;
}

char *Event::serialize()
{
  //fill id_c with FG_EVT_MAX_DEC_LEN digit char rep of id (ie '000012')
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

#ifdef FG_DEBUG
char *Event::human()
{
  switch(type)
  {
    case e_type_ack:            sprintf(event_type_buff,"ack");        break;
    case e_type_assign_con:     sprintf(event_type_buff,"ass_con");    break;
    case e_type_revoke_con:     sprintf(event_type_buff,"rev_con");    break;
    case e_type_refuse_con:     sprintf(event_type_buff,"ref_con");    break;
    case e_type_join_con:       sprintf(event_type_buff,"join_con");   break;
    case e_type_leave_con:      sprintf(event_type_buff,"leave_con");  break;
    case e_type_assign_card:    sprintf(event_type_buff,"ass_card");   break;
    case e_type_revoke_card:    sprintf(event_type_buff,"rev_card");   break;
    case e_type_begin_play:     sprintf(event_type_buff,"begin");      break;
    case e_type_commit_action:  sprintf(event_type_buff,"commit");     break;
    case e_type_commit_actions: sprintf(event_type_buff,"commit_all"); break;
    default:                    sprintf(event_type_buff,"NO HUMAN READABLE EVENT TYPE FOUND"); break;
  }

  sprintf(human_buff,"con:%c card:%c act:%c to:%c what:%c who:%c when:%c where:%c id:%d type:%s", connection, cardinal, action, to, what, who, when, where, id_i, event_type_buff);
  return &human_buff[0];
}
#endif

