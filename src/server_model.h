#ifndef _FG_SERVER_MODEL_H_
#define _FG_SERVER_MODEL_H_

#include "model.h"
#include "network.h"
#include "server.h"

class ServerModel
{
  private :
    Model model;
    Network::Server *server;
  public :
    ServerModel(Network::Server *s);
    ~ServerModel();
    void tick();
};

#endif

