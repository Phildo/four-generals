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
  messenger_id_i = hackatoi(&messenger_id_c[0]);
  id_i = hackatoi(&id_c[0]);
}
Event::Event(char con, char card, char act, char t, char wat, char wo, char wen, char were, char ty, int m_id, int id)
: connection(con), cardinal(card), action(act), to(t), what(wat), who(wo), when(wen), where(were), type(ty), messenger_id_i(m_id), id_i(id)
{
  id_c[0] = '0'; messenger_id_c[0] = '0';
  id_c[1] = '0'; messenger_id_c[1] = '0';
  id_c[2] = '0'; messenger_id_c[2] = '0';
  id_c[3] = '0'; messenger_id_c[3] = '0';
  id_c[4] = '0'; messenger_id_c[4] = '0';
  id_c[5] = '0'; messenger_id_c[5] = '0';
  null = '\0';
}

void Event::zero()
{
  memset(this, '0', sizeof(Event));
  null = '\0';
  messenger_id_i = 0;
  id_i = 0;
}

char *Event::serialize()
{
  //fill id_c with FG_EVT_ID_MAX_DEC_STR_LEN digit char rep of id (ie '000012')
  int tmp_left = id_i;
  int tmp_this = 0;
  for(int i = FG_EVT_ID_MAX_DEC_STR_LEN-1; i > 0; i--)
  {
    tmp_this = tmp_left%10;
    id_c[i] = '0'+tmp_this;
    tmp_left -= tmp_this; //prob don't need to do because of truncation in division
    tmp_left /= 10;
  }

  return (char *)&connection;
}

char *Event::humanAction()
{
  if(action == '0') sprintf(h_action_buff,"");
  if(action == 'a')
  {
    if(who == '0') sprintf(h_action_buff,"Attacking (who)...");
    else           sprintf(h_action_buff,"Attacking %c...",who);
  }
  if(action == 'd')
  {
    sprintf(h_action_buff,"Defending...");
  }
  if(action == 'm')
  {
    if(what == '0')     sprintf(h_action_buff,"Sending Messenger: \"(Attack/Defend)...\"");
    else if(what == 'a')
    {
      if       (who == '0') sprintf(h_action_buff,"Sending Messenger: \"Attack (who)...\"");
      else if (when == '0') sprintf(h_action_buff,"Sending Messenger: \"Attack %c on (when)...\"",who);
      else if(where == '0') sprintf(h_action_buff,"Sending Messenger: \"Attack %c on %c\" to the (where)...",who,when);
      else                  sprintf(h_action_buff,"Sending Messenger: \"Attack %c on %c\" to the %c...",who,when,where);
    }
    else if(what == 'd')
    {
      if      (when == '0') sprintf(h_action_buff,"Sending Messenger: \"Defend on (when)...\"");
      else if(where == '0') sprintf(h_action_buff,"Sending Messenger: \"Defend on %c\" to the (where)...",when);
      else                  sprintf(h_action_buff,"Sending Messenger: \"Defend on %c\" to the %c...",when,where);
    }
  }

  return &h_action_buff[0];
}

#ifdef FG_DEBUG
char *Event::debug()
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
    default:                    sprintf(event_type_buff,"NO DEBUG READABLE EVENT TYPE FOUND"); break;
  }

  sprintf(debug_buff,"con:%c card:%c act:%c to:%c what:%c who:%c when:%c where:%c mid:%d id:%d type:%s", connection, cardinal, action, to, what, who, when, where, messenger_id_i, id_i, event_type_buff);
  return &debug_buff[0];
}
#endif

//yuck
int Event::hackatoi(const char *c)
{
  int r = 0;
  int p;
  for(int i = 0; i < FG_EVT_ID_MAX_DEC_STR_LEN; i++)
  {
    p = ((int)(*(c+(FG_EVT_ID_MAX_DEC_STR_LEN-1-i))-'0'));
    for(int j = 0; j < i; j++) p*=10;
    r += p;
  }
  return r;
}

