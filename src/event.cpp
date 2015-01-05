#include "event.h"

#include "defines.h"
#include "logger.h"
#include "network.h" //serialization string<->int conversions

#include "stdlib.h"
#include <stdio.h> //sprintf

Event::Event() //set to pseudo 'null'
{
  zero();
};
Event::Event(char *c) //MUST MANUALLY KEEP IN SYNC W/ SERIALIZE!
{
  connection = c[0];
  cardinal   = c[1];
  action     = c[2];
  how        = c[3];
  which      = c[4];
  to         = c[5];
  what       = c[6];
  who        = c[7];
  when       = c[8];
  route      = c[9];
  type       = c[10];
  sabotage_id  = String(&c[11],3).intVal();
  messenger_id = String(&c[14],3).intVal();
  evt_id       = String(&c[17],6).intVal();
}

void Event::zero()
{
  connection = '0';
  cardinal   = '0';
  action     = '0';
  how        = '0';
  which      = '0';
  to         = '0';
  what       = '0';
  who        = '0';
  when       = '0';
  route      = '0';
  type       = '0';
  sabotage_id  = 0;
  messenger_id = 0;
  evt_id       = 0;
}

void Event::serialize(char *c) const
{
  String s;
  c[0] = connection;
  c[1] = cardinal;
  c[2] = action;
  c[3] = how;
  c[4] = which;
  c[5] = to;
  c[6] = what;
  c[7] = who;
  c[8] = when;
  c[9] = route;
  c[10] = type;
  s = String::decimalRep(sabotage_id,3);
  c[11] = *(s.ptr()+0);
  c[12] = *(s.ptr()+1);
  c[13] = *(s.ptr()+2);
  s = String::decimalRep(messenger_id,3);
  c[14] = *(s.ptr()+0);
  c[15] = *(s.ptr()+1);
  c[16] = *(s.ptr()+2);
  s = String::decimalRep(evt_id,6);
  c[17] = *(s.ptr()+0);
  c[18] = *(s.ptr()+1);
  c[19] = *(s.ptr()+2);
  c[20] = *(s.ptr()+3);
  c[21] = *(s.ptr()+4);
  c[22] = *(s.ptr()+5);
  c[23] = '\0'; //for good measure
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
      else if(route == '0') sprintf(h_action_buff,"Sending Messenger: \"Attack %c on %c\" to the (route)...",who,when);
      else                  sprintf(h_action_buff,"Sending Messenger: \"Attack %c on %c\" to the %c...",who,when,route);
    }
    else if(what == 'd')
    {
      if      (when == '0') sprintf(h_action_buff,"Sending Messenger: \"Defend on (when)...\"");
      else if(route == '0') sprintf(h_action_buff,"Sending Messenger: \"Defend on %c\" to the (route)...",when);
      else                  sprintf(h_action_buff,"Sending Messenger: \"Defend on %c\" to the %c...",when,route);
    }
  }
  else if(action == 's')
  {
    if(how == '0')      sprintf(h_action_buff,"Sabotaging: \"(Block/Read/Sabotage)...\"");
    else if(how == 'b') sprintf(h_action_buff,"Sabotaging: \"Block...\"");
    else if(how == 'r') sprintf(h_action_buff,"Sabotaging: \"Read...\"");
    else if(how == 's')
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
void Event::debug(char *buff)
{
  switch(type)
  {
    case e_type_join_con:       sprintf(event_type_buff,"join_con");   break;
    case e_type_leave_con:      sprintf(event_type_buff,"leave_con");  break;
    case e_type_assign_card:    sprintf(event_type_buff,"ass_card");   break;
    case e_type_revoke_card:    sprintf(event_type_buff,"rev_card");   break;
    case e_type_begin_play:     sprintf(event_type_buff,"begin");      break;
    case e_type_commit_action:  sprintf(event_type_buff,"commit");     break;
    case e_type_commit_actions: sprintf(event_type_buff,"commit_all"); break;
    case e_type_reset_game:     sprintf(event_type_buff,"reset_game"); break;
    default:                    sprintf(event_type_buff,"NO DEBUG READABLE EVENT TYPE FOUND"); break;
  }

  sprintf(buff,"con:%c card:%c act:%c how:%c which:%c to:%c what:%c who:%c when:%c route:%c mid:%d id:%d type:%c type:%s", connection, cardinal, action, how, which, to, what, who, when, route, messenger_id, evt_id, type, event_type_buff);
}
#endif

