#include "client_model.h"

#include "defines.h"
#include "logger.h"

#include <stdlib.h>

ClientModel::ClientModel(Network::Client *c)
{
  client = c;
  last_known_con = '0';
}

ClientModel::~ClientModel()
{
}

void ClientModel::sendEvent(const Event &e)
{
  Network::Load l;
  e.serialize(l.data);
  client->broadcast(l);
}

void ClientModel::requestJoin()
{
  Event e;
  e.connection = myConnection();
  e.type = e_type_join_con;

  sendEvent(e);
}

void ClientModel::requestCardinal(char card)
{
  Event e;
  e.connection = myConnection();
  e.cardinal = card;
  e.type = e_type_assign_card;

  sendEvent(e);
}

void ClientModel::requestRevokeCardinal()
{
  Event e;
  e.connection = myConnection();
  e.cardinal = myCardinal();
  e.type = e_type_revoke_card;

  sendEvent(e);
}

void ClientModel::requestBeginPlay()
{
  Event e;
  e.connection = myConnection();
  e.cardinal = myCardinal();
  e.type = e_type_begin_play;

  sendEvent(e);
}

void ClientModel::commitAction(Event e)
{
  sendEvent(e);
}



bool ClientModel::imConnected()
{
  return client->con_state == Network::CONNECTION_STATE_CONNECTED && client->con_id != 0;
}

char ClientModel::myConnection()
{
  return '0'+client->con_id;
}

char ClientModel::myCardinal()
{
  return model.connectionCardinal(myConnection());
}

bool ClientModel::imCardinal(char card)
{
  return imConnected() && model.cardinalConnection(card) == myConnection();
}

bool ClientModel::iHaveAction()
{
  return imConnected() && model.connectionHasAction(myConnection());
}

bool ClientModel::iHaveMessage()
{
  return imConnected() && model.connectionHasMessage(myConnection());
}

Messenger& ClientModel::myMessage()
{
  return model.connectionMessage(myConnection());
}

bool ClientModel::iHaveIntruder()
{
  return imConnected() && model.connectionHasIntruder(myConnection());
}

Messenger& ClientModel::myIntruder()
{
  return model.connectionIntruder(myConnection());
}

bool ClientModel::iHaveSabotage()
{
  return imConnected() && iHaveIntruder() && myIntruder().sabotaged == 'r';
}

Messenger& ClientModel::mySabotage()
{
  return myIntruder();
}

bool ClientModel::iWin()
{
  return model.connectionWin(myConnection());
}

bool ClientModel::iLose()
{
  return model.connectionLose(myConnection());
}

void ClientModel::tick()
{
  Network::Load l;

  if(imConnected() && last_known_con == '0')
  {
    requestJoin();
    last_known_con = myConnection();
  }

  while(client->read(l))
  {
    Event e(l.data);
    switch(e.type)
    {
      case e_type_join_con:
        model.connectCon(e.connection);
        break;
      case e_type_leave_con:
        model.disconnectCon(e.connection);
        break;
      case e_type_assign_card:
        model.assignConCard(e.connection, e.cardinal);
        break;
      case e_type_revoke_card:
        model.revokeCard(e.cardinal);
        break;
      case e_type_begin_play:
        model.days = 0;
        break;
      case e_type_commit_action:
        model.assignConAction(e.connection, e);
        break;
      case e_type_commit_actions:
        model.commitActions(); //also increases day
        break;
      default:
        break;
    }
  }
}

