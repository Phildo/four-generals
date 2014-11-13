#ifndef _FG_SERVER_MODEL_H_
#define _FG_SERVER_MODEL_H_

#include "model.h"
#include "network.h"
#include "server.h"

class ServerModel
{
  private :
    Network::Server *server;
  public :
    Model model;
    int messenger_id; //needs to take authority in assigning ids
    int sabotage_id;  //needs to take authority in assigning ids
    ServerModel(Network::Server *s);
    ~ServerModel();
    void tick();
    int nextMessId();
    int nextSabId();
};

#endif

