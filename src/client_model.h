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

    ClientModel(Network::Client *c);
    ~ClientModel();

    bool imConnected();
    char myConnection();
    char myCardinal();
    bool imCardinal(char card);
    bool iHaveAction();
    bool iHaveMessage();
    Messenger& myMessage();
    bool iHaveIntruder();
    Messenger& myIntruder();

    void tick();
};

#endif

