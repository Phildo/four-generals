#include "client_model.h"

#include "client.h"
#include "event.h"

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

General *ClientModel::cardGeneral(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return &generals[i];
  return 0;
}

General *ClientModel::conGeneral(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return &generals[i];
  return 0;
}

General *ClientModel::emptyGeneral()
{
  return conGeneral('0');
}

void ClientModel::tick()
{
  Network::Event *e;
  while((e = client->getEvent()))
  {
    switch(e->type)
    {
      case Network::e_type_ack: break;        //should never reach model (handled entirely by client)
      case Network::e_type_assign_con: break; //should never reach model (handled entirely by client)
      case Network::e_type_revoke_con: break; //should never reach model (handled entirely by client)
      case Network::e_type_refuse_con: break; //should never reach model (handled entirely by client)
      case Network::e_type_join_con:
        emptyGeneral()->connection = e->connection;
        break;
      case Network::e_type_leave_con:
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
    fg_log("ClientModel sees %s",e->serialize());
  }

}

