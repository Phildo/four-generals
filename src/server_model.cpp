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
      e.connection = '0'+i;
      e.type = e_type_leave_con;
      model.disconnectCon(e.connection);
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
        model.connectCon(e.connection);
        dumpHistory(e.connection);
        sendEvent(e);
        break;
      case e_type_leave_con:
        model.disconnectCon(e.connection);
        sendEvent(e);
        break;
      case e_type_assign_card:
        if(!model.cardinalConnected(e.cardinal))
        {
          model.assignConCard(e.connection, e.cardinal);
          sendEvent(e);
        }
        break;
      case e_type_revoke_card:
        if(model.cardinalConnected(e.cardinal) && model.cardinalGeneral(e.cardinal).connection == e.connection)
        {
          model.revokeCard(e.cardinal);
          sendEvent(e);
        }
        break;
      case e_type_begin_play:
        if(model.rolesAssigned())
        {
          model.days = 0;
          sendEvent(e);
        }
        break;
      case e_type_commit_action:
        if(!model.connectionHasAction(e.connection))
        {
          //inject assignment of messenger id here
          if(e.action == 'm') e.messenger_id = mess_id_store.getId();
          if(e.action == 's') e.sabotage_id  = sabo_id_store.getId();
          model.assignConAction(e.connection, e);
          sendEvent(e);
        }
        if(model.actionsAssigned())
        {
          model.commitActions(); //also increases day
          e.type = e_type_commit_actions; //give goahead for mass commit
          sendEvent(e);
        }
        break;
      case e_type_commit_actions: break; //server->client only
      case e_type_reset_game:
        model.zero();
        sendEvent(e);
        break;
      default: break;
    }
  }
}

