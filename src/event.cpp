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
Event::Event(char con, char card, char act, char ow, char wich, char t, char wat, char wo, char wen, char were, char ty, int s_id, int m_id, int id)
: connection(con), cardinal(card), action(act), how(ow), which(wich), to(t), what(wat), who(wo), when(wen), where(were), type(ty), sabotage_id_i(s_id), messenger_id_i(m_id), id_i(id)
{
  for(int i = 0; i < FG_EVT_ID_MAX_DEC_STR_LEN; i++)
  {
    id_c[i]           = '0';
    sabotage_id_c[i]  = '0';
    messenger_id_c[i] = '0';
  }
  null = '\0';
}

void Event::zero()
{
  memset(this, '0', sizeof(Event));
  null = '\0';
  sabotage_id_i = 0;
  messenger_id_i = 0;
  id_i = 0;
}

char *Event::serialize()
{
  hackitoa(messenger_id_i,messenger_id_c);
  hackitoa(id_i,id_c);
  return (char *)&connection; //holy hacks on hacks on hacks
}

char *Event::humanAction()
{
  if(action == '0') sprintf(h_action_buff,"");
  else if(action == 'a')
  {
    if(who == '0') sprintf(h_action_buff,"Attacking (who)...");
    else           sprintf(h_action_buff,"Attacking %c...",who);
  }
  else if(action == 'd')
  {
    sprintf(h_action_buff,"Defending...");
  }
  else if(action == 'm')
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
  else if(action == 's')
  {
    if(how == '0')     sprintf(h_action_buff,"Sabotaging: \"(Block/Read/Sabotage)...\"");
    else if(how == 'b') sprintf(h_action_buff,"Sabotaging: \"Block...\"");
    else if(how == 'r') sprintf(h_action_buff,"Sabotaging: \"Read...\"");
    else if(how == 's') sprintf(h_action_buff,"Sabotaging: \"Read...\"");
    {
      if(which == '0') sprintf(h_action_buff,"Sabotaging: \"Sabotage (what)...\"");
      else if(which == 'a')
      {
        if(what == '0') sprintf(h_action_buff,"Sabotaging: \"Sabotage what to ...");
        else            sprintf(h_action_buff,"Sabotaging: \"Sabotage what to '%c'...",what);
      }
      else if(which == 'o')
      {
        if(who == '0') sprintf(h_action_buff,"Sabotaging: \"Sabotage who to ...");
        else           sprintf(h_action_buff,"Sabotaging: \"Sabotage who to '%c'...",who);
      }
      else if(which == 'e')
      {
        if(when == '0') sprintf(h_action_buff,"Sabotaging: \"Sabotage when to ...");
        else            sprintf(h_action_buff,"Sabotaging: \"Sabotage when to '%c'...",when);
      }
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

  sprintf(debug_buff,"con:%c card:%c act:%c how:%c which:%c to:%c what:%c who:%c when:%c where:%c mid:%d id:%d type:%s", connection, cardinal, action, how, which, to, what, who, when, where, messenger_id_i, id_i, event_type_buff);
  return &debug_buff[0];
}
#endif


//yuck
void Event::hackitoa(int i, char *c)
{
  //fill id_c with FG_EVT_ID_MAX_DEC_STR_LEN digit char rep of id (ie '000012')
  int d = 0;
  for(int j = FG_EVT_ID_MAX_DEC_STR_LEN-1; j > 0; j--)
  {
    d = i%10;
    c[j] = '0'+d;
    i -= d; //prob don't need to do because of truncation in division
    i /= 10;
  }
}

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

