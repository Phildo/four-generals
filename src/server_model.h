#ifndef _FG_SERVER_MODEL_H_
#define _FG_SERVER_MODEL_H_

#include "general.h"
#include "network.h"

class ServerModel
{
  private :
    Network::Server *server;
  public :
    General generals[4];
    General *cardGeneral(char card);
    General *conGeneral(char con);
    General *emptyGeneral();

    ServerModel(Network::Server *s);
    ~ServerModel();
    void tick();
};

#endif

