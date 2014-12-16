#ifndef _FG_CLIENT_MODEL_H_
#define _FG_CLIENT_MODEL_H_

#include "model.h"
#include "network.h"
#include "client.h"

class ClientModel
{
  private :
    Network::Client *client;

    char last_known_con; //hack to be aware of disconnect
    void sendEvent(const Event &e);
  public :
    Model model;

    ClientModel(Network::Client *c);
    ~ClientModel();

    void requestJoin();
    void requestCardinal(char card);
    void requestRevokeCardinal();
    void requestBeginPlay();
    void commitAction(Event e);
    void requestReset();

    bool imConnected();
    char myConnection();
    char myCardinal();
    bool imCardinal(char card);
    bool iHaveAction();
    bool iHaveMessage();
    Messenger myMessage();
    bool iHaveIntruder();
    Messenger myIntruder();
    bool iHaveSabotage();
    Messenger mySabotage();
    bool iWin();
    bool iLose();
    bool iTie();

    void tick();
};

#endif

