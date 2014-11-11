#include "server_model.h"

#include "defines.h"
#include "logger.h"

#include <stdlib.h>

ServerModel::ServerModel(Network::Server *s)
{
  server = s;
  messenger_id = 0;
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
        server->broadcast(e); //alert others of joined player
        break;
      case e_type_leave_con:
        model.disconnectCon(e->connection);
        server->broadcast(e); //alert others of joined player
        break;
      case e_type_assign_card:
        if(!model.cardinalConnected(e->cardinal))
        {
          model.assignConCard(e->connection, e->cardinal);
          server->broadcast(e);
        }
        break;
      case e_type_revoke_card:
        if(model.cardinalConnected(e->cardinal) && model.cardinalGeneral(e->cardinal).connection == e->connection)
        {
          model.revokeCard(e->cardinal);
          server->broadcast(e);
        }
        break;
      case e_type_begin_play:
        if(model.rolesAssigned())
        {
          model.days = 0;
          server->broadcast(e);
        }
        break;
      case e_type_commit_action:
        if(!model.connectionHasAction(e->connection))
        {
          //inject assignment of messenger id here
          if(e->action == 'm') e->messenger_id_i = nextMessId();
          model.assignConAction(e->connection, *e);
          server->broadcast(e);
        }
        if(model.actionsAssigned())
        {
          model.commitActions(); //also increases day
          e->type = e_type_commit_actions; //give goahead for mass commit
          server->broadcast(e);
        }
        break;
      case e_type_commit_actions: break; //server->client only
      default: break;
    }
  }
}

int ServerModel::nextMessId()
{
  messenger_id++;
  return messenger_id;
}

