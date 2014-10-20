#ifndef _FG_MODEL_H_
#define _FG_MODEL_H_

#include "general.h"
#include "network.h"

class Model
{
  private :
    Network::Client *client;
  public :
    General generals[4];
    General *cardGeneral(char card);
    General *conGeneral(char con);
    General *emptyGeneral();

    Model(Network::Client *c);
    ~Model();
    void tick();
};

#endif

