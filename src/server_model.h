#ifndef _FG_SERVER_MODEL_H_
#define _FG_SERVER_MODEL_H_

#include "model.h"
#include "network.h"
#include "server.h"
#include "id_store.h"

class ServerModel
{
  private :
    Network::Server *server;
    IdStore<256> mess_id_store;
    IdStore<256> sabo_id_store;
  public :
    Model model;
    ServerModel(Network::Server *s);
    ~ServerModel();
    void tick();
};

#endif

