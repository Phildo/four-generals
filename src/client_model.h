#ifndef _FG_CLIENT_MODEL_H_
#define _FG_CLIENT_MODEL_H_

#include "model.h"
#include "network.h"
#include "client.h"

class ClientModel
{
  private :
    Network::Client *client;
  public :
    ClientModel(Network::Client *c);
    ~ClientModel();
    void tick();
};

#endif

