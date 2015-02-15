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

void ClientModel::sendEvent(const Event &e)
{
  Network::Load l;
  e.serialize(l.data);
  client->broadcast(l);
}

void ClientModel::requestJoin()
{
  Event e;
  e.type = e_type_join_con;
  e.join_con.connection = myConnection();

  sendEvent(e);
}

void ClientModel::requestCardinal(char card)
{
  Event e;
  e.type = e_type_assign_card;
  e.assign_card.connection = myConnection();
  e.assign_card.cardinal = card;

  sendEvent(e);
}

void ClientModel::requestRevokeCardinal()
{
  Event e;
  e.type = e_type_revoke_card;
  e.revoke_card.connection = myConnection();
  e.revoke_card.cardinal = myCardinal();

  sendEvent(e);
}

void ClientModel::requestBeginPlay()
{
  Event e;
  e.type = e_type_begin_play;
  e.begin_play.null = '\0';

  sendEvent(e);
}

void ClientModel::commitTurn(Turn t)
{
  Event e;
  t.cardinal = myCardinal();
  e.type = e_type_commit_turn;
  e.commit_turn.connection = myConnection();
  e.commit_turn.turn = t;

  sendEvent(e);
}

void ClientModel::requestReset()
{
  Event e;
  e.type = e_type_reset_game;
  e.reset_game.null = '\0';

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

bool ClientModel::iHaveTurn()
{
  return imConnected() && model.cardinalHasTurn(myCardinal());
}

bool ClientModel::myMessage(Action& a)
{
  if(!imConnected())
  {
    a.zero();
    return false;
  }
  else return model.cardinalMessage(myCardinal(),a);
}

bool ClientModel::myIntruder(Action& a0, Action&a1)
{
  if(!imConnected())
  {
    a0.zero();
    a1.zero();
    return false;
  }
  else return model.cardinalIntruder(myCardinal(),a0,a1);
}

bool ClientModel::mySabotage(Action& am, Action& a0, Action& a1)
{
  if(!imConnected())
  {
    am.zero();
    a0.zero();
    a1.zero();
    return false;
  }
  else return model.cardinalSabotage(myCardinal(),am,a0,a1);
}

bool ClientModel::iWin()
{
  return model.cardinalWin(myCardinal());
}

bool ClientModel::iLose()
{
  return model.cardinalLose(myCardinal());
}

bool ClientModel::iTie()
{
  return model.cardinalTie(myCardinal());
}

void ClientModel::tick()
{
  Network::Load l;

  static bool already_tried_joining = false;
  if(imConnected() && !model.connectionConnected(myConnection()) && !already_tried_joining)
  {
    requestJoin();
    already_tried_joining = true;
  }

  while(client->read(l))
  {
    Event e(l.data);
    switch(e.type)
    {
      case e_type_join_con:
        model.connectCon(e.join_con.connection);
        break;
      case e_type_leave_con:
        model.disconnectCon(e.leave_con.connection);
        break;
      case e_type_assign_card:
        model.assignConCard(e.assign_card.connection, e.assign_card.cardinal);
        break;
      case e_type_revoke_card:
        model.revokeCard(e.revoke_card.cardinal);
        break;
      case e_type_begin_play:
        model.days = 0;
        break;
      case e_type_commit_turn:
        model.assignCardinalTurn(model.connectionCardinal(e.commit_turn.connection), e.commit_turn.turn);
        break;
      case e_type_commit_turns:
        model.commitTurns(); //also increases day
        break;
      case e_type_reset_game:
        model.zeroRound();
        break;
      default:
        break;
    }
  }
}

