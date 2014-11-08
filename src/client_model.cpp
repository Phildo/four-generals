#include "client_model.h"

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

bool ClientModel::imConnected()
{
  return connection != '0';
}

bool ClientModel::imCardinal(char card)
{
  return imConnected() && model.cardToCon('n') == connection;
}

bool ClientModel::cardinalConnected(char card)
{
  return model.cardToCon('n') != '0';
}

void ClientModel::tick()
{
  Event *e;
  Event *a;

  while((e = client->getEvent()))
  {
    switch(e->type)
    {
      case e_type_ack: break;        //should never reach model (handled entirely by client)
      case e_type_assign_con: break; //should never reach model (handled entirely by client)
      case e_type_revoke_con: break; //should never reach model (handled entirely by client)
      case e_type_refuse_con: break; //should never reach model (handled entirely by client)
      case e_type_join_con:
        model.connections[model.iconnection(e->connection)] = e->connection;
        break;
      case e_type_leave_con:
        model.connections[model.iconnection(e->connection)] = '0';
        break;
      case e_type_assign_card:
        model.generals[model.compass.icardinal(e->cardinal)].connection = e->connection;
        model.generals[model.compass.icardinal(e->cardinal)].cardinal   = e->cardinal;
        break;
      case e_type_revoke_card:
        model.generals[model.compass.icardinal(e->cardinal)].connection = '0';
        model.generals[model.compass.icardinal(e->cardinal)].cardinal   = '0';
        break;
      case e_type_begin_play:
        model.days = 0;
        break;
      case e_type_commit_action:
        /*
        a = conAction(e->connection);
        *a = *e; //lol
        */
        break;
      case e_type_set_day:
        /*
        cardAction('n')->zero();
        cardAction('e')->zero();
        cardAction('s')->zero();
        cardAction('w')->zero();
        day = e->when;
        */
        break;
      default:
        break;
    }
  }
}

