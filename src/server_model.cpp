#include "server_model.h"

#include "server.h"
#include "event.h"

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

General *ServerModel::cardGeneral(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return &generals[i];
  return 0;
}

General *ServerModel::conGeneral(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return &generals[i];
  return 0;
}

General *ServerModel::emptyGeneral()
{
  return conGeneral('0');
}

void ServerModel::tick()
{
  Network::Event *e;
  while((e = server->getEvent()))
  {
    fg_log("ServerModel: %s",e->serialize());
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
           cardGeneral('w')
          )
        {
          playing = true;
          server->broadcast(*e);
        }
        break;
      default:
        break;
    }
  }
}

