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
    Model model;
    char connection;

    ClientModel(Network::Client *c);
    ~ClientModel();

    bool imConnected();
    bool imCardinal(char card);
    bool cardinalConnected(char card);

    void tick();
};

#endif

