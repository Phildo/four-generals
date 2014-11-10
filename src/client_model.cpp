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
  return client->connection != '0';
}

char ClientModel::myConnection()
{
  return client->connection;
}

char ClientModel::myCardinal()
{
  return model.connectionCardinal(client->connection);
}

bool ClientModel::imCardinal(char card)
{
  return imConnected() && model.cardinalConnection(card) == client->connection;
}

bool ClientModel::iHaveAction()
{
  return imConnected() && model.connectionHasAction(client->connection);
}

bool ClientModel::iHaveMessage()
{
  return imConnected() && model.connectionHasMessage(client->connection);
}

Messenger& ClientModel::myMessage()
{
  return model.connectionMessage(client->connection);
}

bool ClientModel::iHaveIntruder()
{
  return imConnected() && model.connectionHasIntruder(client->connection);
}

Messenger& ClientModel::myIntruder()
{
  return model.connectionIntruder(client->connection);
}

void ClientModel::tick()
{
  Event *e;

  while((e = client->getEvent()))
  {
    switch(e->type)
    {
      case e_type_ack: break;        //should never reach model (handled entirely by client)
      case e_type_assign_con: break; //should never reach model (handled entirely by client)
      case e_type_revoke_con: break; //should never reach model (handled entirely by client)
      case e_type_refuse_con: break; //should never reach model (handled entirely by client)
      case e_type_join_con:
        model.connectCon(e->connection);
        break;
      case e_type_leave_con:
        model.disconnectCon(e->connection);
        break;
      case e_type_assign_card:
        model.assignConCard(e->connection, e->cardinal);
        break;
      case e_type_revoke_card:
        model.revokeCard(e->cardinal);
        break;
      case e_type_begin_play:
        model.days = 0;
        break;
      case e_type_commit_action:
        model.assignConAction(e->connection, *e);
        break;
      case e_type_commit_actions:
        model.commitActions(); //also increases day
        break;
      default:
        break;
    }
  }
}

