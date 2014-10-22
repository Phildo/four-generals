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
  return conGeneral(0);
}

void ServerModel::tick()
{
  Network::Event *e;
  while((e = server->getEvent()))
  {
    switch(e->type)
    {
      case Network::e_type_assign_con:
        generals[0].connection = e->connection;
        break;
      case Network::e_type_refuse_con:
        break;
      case Network::e_type_join:
        emptyGeneral()->connection = e->connection;
        break;
      case Network::e_type_leave:
        conGeneral(e->connection)->connection = '0';
        break;
      case Network::e_type_assign_card:
        conGeneral(e->connection)->cardinal = e->cardinal;
        break;
      case Network::e_type_revoke_card:
        conGeneral(e->connection)->cardinal = '0';
        break;
      default:
        break;
    }
    fg_log("ServerModel sees %s",e->serialize());
  }
}

