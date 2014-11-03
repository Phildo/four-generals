#include "client_model.h"

#include "client.h"

#include "defines.h"
#include "logger.h"

#include <stdlib.h>

ClientModel::ClientModel(Network::Client *c)
{
  client = c;
}

ClientModel::~ClientModel()
{
}

int ClientModel::cardToIndex(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return i;
  return -1;
}

General *ClientModel::cardGeneral(char card)
{
  int i = cardToIndex(card);
  if(i != -1) return &generals[i];
  return 0;
}

Network::Event *ClientModel::cardAction(char card)
{
  int i = cardToIndex(card);
  if(i != -1) return &actions[i];
  return 0;
}

int ClientModel::conToIndex(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return i;
  return -1;
}

General *ClientModel::conGeneral(char con)
{
  int i = conToIndex(con);
  if(i != -1) return &generals[i];
  return 0;
}

Network::Event *ClientModel::conAction(char con)
{
  int i = conToIndex(con);
  if(i != -1) return &actions[i];
  return 0;
}

General *ClientModel::emptyGeneral()
{
  return conGeneral('0');
}

void ClientModel::tick()
{
  Network::Event *e;
  Network::Event *a;
  while((e = client->getEvent()))
  {
    switch(e->type)
    {
      case Network::e_type_ack: break;        //should never reach model (handled entirely by client)
      case Network::e_type_assign_con: break; //should never reach model (handled entirely by client)
      case Network::e_type_revoke_con: break; //should never reach model (handled entirely by client)
      case Network::e_type_refuse_con: break; //should never reach model (handled entirely by client)
      case Network::e_type_join_con:
        emptyGeneral()->connection = e->connection;
        break;
      case Network::e_type_leave_con:
        conGeneral(e->connection)->cardinal = '0';
        conGeneral(e->connection)->connection = '0';
        break;
      case Network::e_type_assign_card:
        conGeneral(e->connection)->cardinal = e->cardinal;
        break;
      case Network::e_type_revoke_card:
        conGeneral(e->connection)->cardinal = '0';
        break;
      case Network::e_type_begin_play:
        playing = true;
        day = 's';
        break;
      case Network::e_type_commit_action:
        a = conAction(e->connection);
        *a = *e; //lol
        break;
      default:
        break;
    }
  }
}

