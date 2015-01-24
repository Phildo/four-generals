#include "server_model.h"

#include "defines.h"
#include "logger.h"

#include <stdlib.h>

ServerModel::ServerModel(Network::Server *s)
{
  server = s;
  hist_i = 0;
}

ServerModel::~ServerModel()
{
}

void ServerModel::sendEvent(const Event &e)
{
  if(hist_i < 256) history[hist_i++] = e;

  Network::Load l;
  e.serialize(l.data);
  server->broadcast(l);
}

void ServerModel::dumpHistory(char con)
{
  for(int i = 0; i < hist_i; i++)
  {
    Network::Load l;
    history[i].serialize(l.data);
    l.con_id = con-'0';
    server->individualBroadcast(l);
  }
}

void ServerModel::tick()
{
  Network::Load l;

  for(int i = 1; i <= 5; i++)
  {
    //report any disconnections
    if(model.connectionConnection('0'+i) != '0' && !server->hasConnection(i))
    {
      Event e;
      e.type = e_type_leave_con;
      e.leave_con.connection = '0'+i;
      model.disconnectCon(e.leave_con.connection);
      sendEvent(e);
    }
  }

  //server model's job- 1.verify 2.process 3.broadcast
  while(server->read(l))
  {
    Event e(l.data);
    switch(e.type)
    {
      case e_type_join_con:
        model.connectCon(e.join_con.connection);
        dumpHistory(e.join_con.connection);
        sendEvent(e);
        break;
      case e_type_leave_con:
        model.disconnectCon(e.leave_con.connection);
        sendEvent(e);
        break;
      case e_type_assign_card:
        if(!model.cardinalConnected(e.assign_card.cardinal))
        {
          model.assignConCard(e.assign_card.connection, e.assign_card.cardinal);
          sendEvent(e);
        }
        break;
      case e_type_revoke_card:
        if(model.cardinalConnected(e.revoke_card.cardinal) && model.cardinalGeneral(e.revoke_card.cardinal).connection == e.revoke_card.connection)
        {
          model.revokeCard(e.revoke_card.cardinal);
          sendEvent(e);
        }
        break;
      case e_type_begin_play:
        //NOTE- PHIL HACK HACKED THIS FOR FAKE HOST SCENE
        //if(model.rolesAssigned())
        //{
          model.days = 0;
          sendEvent(e);
        //}
        break;
      case e_type_commit_turn:
        if(!model.cardinalHasTurn(model.connectionCardinal(e.commit_turn.connection)))
        {
          model.assignCardinalTurn(model.connectionCardinal(e.commit_turn.connection), e.commit_turn.turn);
          sendEvent(e);
        }
        if(model.turnsAssigned())
        {
          model.commitTurns(); //also increases day
          e.type = e_type_commit_turns; //give goahead for mass commit
          sendEvent(e);
        }
        break;
      case e_type_commit_turns: break; //server->client only
      case e_type_reset_game:
        model.zeroRound();
        sendEvent(e);
        break;
      default: break;
    }
  }
}

