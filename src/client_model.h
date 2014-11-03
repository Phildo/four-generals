#ifndef _FG_CLIENT_MODEL_H_
#define _FG_CLIENT_MODEL_H_

#include "general.h"
#include "network.h"
#include "event.h"

class ClientModel
{
  private :
    Network::Client *client;
  public :
    General generals[4];
    Network::Event actions[4];

    int cardToIndex(char card);
    General *cardGeneral(char card);
    Network::Event *cardAction(char card);

    int conToIndex(char con);
    General *conGeneral(char con);
    Network::Event *conAction(char con);

    General *emptyGeneral();

    bool playing; //dumb
    int day;

    ClientModel(Network::Client *c);
    ~ClientModel();
    void tick();
};

#endif

