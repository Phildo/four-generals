#include "server_model.h"

#include "defines.h"
#include "logger.h"

#include <stdlib.h>

ServerModel::ServerModel(Network::Server *s)
{
  server = s;
}

ServerModel::~ServerModel()
{
}

void ServerModel::tick()
{
  Event *e;

  //server model's job- 1.verify 2.process 3.broadcast
  while((e = server->getEvent()))
  {
    switch(e->type)
    {
      case e_type_ack: break; //should never reach model (handled entirely by server)
      case e_type_assign_con: break; //server->client only
      case e_type_revoke_con: break; //server->client only
      case e_type_refuse_con: break; //server->client only
      case e_type_join_con:
        model.connectCon(e->connection);
        server->broadcast(*e); //alert others of joined player
        break;
      case e_type_leave_con:
        model.disconnectCon(e->connection);
        server->broadcast(*e); //alert others of joined player
        break;
      case e_type_assign_card:
        if(!model.cardinalConnected(e->cardinal))
        {
          model.assignConCard(e->connection, e->cardinal);
          server->broadcast(*e);
        }
        break;
      case e_type_revoke_card:
        if(model.cardinalConnected(e->cardinal) && model.cardinalGeneral(e->cardinal).connection == e->connection)
        {
          model.revokeCard(e->cardinal);
          server->broadcast(*e);
        }
        break;
      case e_type_begin_play:
        if(model.rolesAssigned())
        {
          model.days = 0;
          server->broadcast(*e);
        }
        break;
      case e_type_commit_action:
        /*
        a = conAction(e->connection);
        *a = *e; //lol
        server->broadcast(*e);

        if(cardAction('n')->type == e_type_commit_action &&
           cardAction('e')->type == e_type_commit_action &&
           cardAction('s')->type == e_type_commit_action &&
           cardAction('w')->type == e_type_commit_action)
        {
          //go to next day

          //process events
          for(int i = 0; i < 4; i++)
          {
            char c;
            if(i == 0) c = 'n';
            if(i == 1) c = 'e';
            if(i == 2) c = 's';
            if(i == 3) c = 'w';
            a = cardAction(c);
            if(a->action == 'a')
            {
              if(cardAction(a->who)->action != 'd' &&
                 cardAction(cardToPartner(c))->action == 'a')
              {
                //win
              }
              else if(cardAction(a->who)->action == 'd' &&
                      cardAction(cardToPartner(c))->action != 'a')
              {
                //lose
              }
            }
            else if(a->action == 'm')
            {
              m = Messenger(*e);
              messengers.add(m);
            }
          }

          //update messengers
          for(int i = 0; i < messengers.length(); i++)
          {
            if(messengers[i].at == messengers[i].to) //last
              messengers.remove(i); i--; //display message
            if(messengers[i].at == messengers[i].where) //middle
              messengers[i].at = messengers[i].to; //allow sabotage
            if(messengers[i].at == messengers[i].from) //first
              messengers[i].at = messengers[i].where;
          }

          cardAction('n')->zero();
          cardAction('e')->zero();
          cardAction('s')->zero();
          cardAction('w')->zero();

          days++;

          event.zero();
          event.type = e_type_set_day;
          event.when = dayForDays(days);
          server->broadcast(event);
        }
        */
        break;
      case e_type_set_day: break; //only received by client
      default: break;
    }
  }
}

