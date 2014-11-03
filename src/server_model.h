#ifndef _FG_SERVER_MODEL_H_
#define _FG_SERVER_MODEL_H_

#include "general.h"
#include "network.h"
#include "event.h"

class ServerModel
{
  private :
    Network::Server *server;
  public :
    General generals[4];
    Network::Event actions[4]; //nesw

    int cardToIndex(char card);
    General *cardGeneral(char card);
    Network::Event *cardAction(char card);

    int conToIndex(char con);
    General *conGeneral(char con);
    Network::Event *conAction(char con);

    General *emptyGeneral();

    char dayForDays(int d);

    bool playing; //dumb
    int days;

    ServerModel(Network::Server *s);
    ~ServerModel();
    void tick();
};

#endif

