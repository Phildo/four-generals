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
  Network::Load l;

  //server model's job- 1.verify 2.process 3.broadcast
  while(server->read(l))
  {
    Event e(l.data);
    switch(e.type)
    {
      case e_type_join_con:
        model.connectCon(e.connection);
        server->broadcast(l); //alert others of joined player
        break;
      case e_type_leave_con:
        model.disconnectCon(e.connection);
        server->broadcast(l); //alert others of joined player
        break;
      case e_type_assign_card:
        if(!model.cardinalConnected(e.cardinal))
        {
          model.assignConCard(e.connection, e.cardinal);
          server->broadcast(l);
        }
        break;
      case e_type_revoke_card:
        if(model.cardinalConnected(e.cardinal) && model.cardinalGeneral(e.cardinal).connection == e.connection)
        {
          model.revokeCard(e.cardinal);
          server->broadcast(l);
        }
        break;
      case e_type_begin_play:
        if(model.rolesAssigned())
        {
          model.days = 0;
          server->broadcast(l);
        }
        break;
      case e_type_commit_action:
        if(!model.connectionHasAction(e.connection))
        {
          //inject assignment of messenger id here
          if(e.action == 'm') e.messenger_id = mess_id_store.getId();
          if(e.action == 's') e.sabotage_id  = sabo_id_store.getId();
          model.assignConAction(e.connection, e);
          server->broadcast(l);
        }
        if(model.actionsAssigned())
        {
          model.commitActions(); //also increases day
          e.type = e_type_commit_actions; //give goahead for mass commit
          e.serialize(l.data);
          server->broadcast(l);
        }
        break;
      case e_type_commit_actions: break; //server->client only
      default: break;
    }
  }
}

