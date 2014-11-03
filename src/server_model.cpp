#include "server_model.h"

#include "server.h"

#include "defines.h"
#include "logger.h"

#include <stdlib.h>

ServerModel::ServerModel(Network::Server *s)
{
  server = s;
  days = 0;
}

ServerModel::~ServerModel()
{
}

int ServerModel::cardToIndex(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return i;
  return -1;
}

General *ServerModel::cardGeneral(char card)
{
  int i = cardToIndex(card);
  if(i != -1) return &generals[i];
  return 0;
}

Network::Event *ServerModel::cardAction(char card)
{
  int i = cardToIndex(card);
  if(i != -1) return &actions[i];
  return 0;
}

int ServerModel::conToIndex(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return i;
  return -1;
}

General *ServerModel::conGeneral(char con)
{
  int i = conToIndex(con);
  if(i != -1) return &generals[i];
  return 0;
}

Network::Event *ServerModel::conAction(char con)
{
  int i = conToIndex(con);
  if(i != -1) return &actions[i];
  return 0;
}

General *ServerModel::emptyGeneral()
{
  return conGeneral('0');
}

char ServerModel::dayForDays(int d)
{
  int i = d%7;
  if(i == 0) return 's';
  if(i == 1) return 'm';
  if(i == 2) return 't';
  if(i == 3) return 'w';
  if(i == 4) return 'h';
  if(i == 5) return 'f';
  if(i == 6) return 'a';
  return '0'; //impossible
}

void ServerModel::tick()
{
  Network::Event *e;
  Network::Event *a;
  Network::Event event;
  while((e = server->getEvent()))
  {
    switch(e->type)
    {
      case Network::e_type_ack: break; //should never reach model (handled entirely by server)
      case Network::e_type_assign_con: break; //only received by client
      case Network::e_type_revoke_con: break; //only received by client
      case Network::e_type_refuse_con: break; //only received by client
      case Network::e_type_join_con:
        if(conGeneral(e->connection) == 0)
        {
          emptyGeneral()->connection = e->connection;
          server->broadcast(*e); //alert others of joined player
        }
        break;
      case Network::e_type_leave_con:
        conGeneral(e->connection)->cardinal = '0';
        conGeneral(e->connection)->connection = '0';
        server->broadcast(*e); //alert others of left player
        break;
      case Network::e_type_assign_card:
        if(cardGeneral(e->cardinal) == 0)
        {
          conGeneral(e->connection)->cardinal = e->cardinal;
          server->broadcast(*e);
        }
        break;
      case Network::e_type_revoke_card:
        if(cardGeneral(e->cardinal)->connection == e->connection)
        {
          conGeneral(e->connection)->cardinal = '0';
          server->broadcast(*e);
        }
        break;
      case Network::e_type_begin_play:
        if(cardGeneral('n') &&
           cardGeneral('e') &&
           cardGeneral('s') &&
           cardGeneral('w'))
        {
          playing = true;
          days = 0;
          server->broadcast(*e);
        }
        break;
      case Network::e_type_commit_action:
        a = conAction(e->connection);
        *a = *e; //lol
        server->broadcast(*e);

        if(cardAction('n')->type == Network::e_type_commit_action &&
           cardAction('e')->type == Network::e_type_commit_action &&
           cardAction('s')->type == Network::e_type_commit_action &&
           cardAction('w')->type == Network::e_type_commit_action)
        {
          //go to next day

          //process events

          cardAction('n')->zero();
          cardAction('e')->zero();
          cardAction('s')->zero();
          cardAction('w')->zero();

          days++;

          event.zero();
          event.type = Network::e_type_set_day;
          event.when = dayForDays(days);
          server->broadcast(event);
        }
        break;
      case Network::e_type_set_day: break; //only received by client
      default:
        break;
    }
  }
}

