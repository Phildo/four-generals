#ifndef _FG_CLIENT_MODEL_H_
#define _FG_CLIENT_MODEL_H_

#include "general.h"
#include "network.h"

class ClientModel
{
  private :
    Network::Client *client;
  public :
    General generals[4];
    General *cardGeneral(char card);
    General *conGeneral(char con);
    General *emptyGeneral();

    bool playing; //dumb
    char day;

    ClientModel(Network::Client *c);
    ~ClientModel();
    void tick();
};

#endif

