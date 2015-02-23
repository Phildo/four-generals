#ifndef _FG_CLIENT_MODEL_H_
#define _FG_CLIENT_MODEL_H_

#include "model.h"
#include "network.h"
#include "client.h"

#include "event.h"

class ClientModel
{
  private :
    Network::Client *client;
    bool already_tried_joining; //bad

    void sendEvent(const Event &e);
  public :
    Model model;

    ClientModel(Network::Client *c);
    ~ClientModel();

    void requestJoin();
    void requestCardinal(char card);
    void requestRevokeCardinal();
    void requestBeginPlay();
    void commitTurn(Turn t);
    void requestReset();

    bool imConnected();
    char myConnection();
    char myCardinal();
    bool imCardinal(char card);
    bool iHaveTurn();

    bool myMessage(Action& a);
    bool myIntruder(Action& a0, Action& a1);
    bool mySabotage(Action& am, Action& a0, Action& a1);

    bool iWin();
    bool iLose();
    bool iTie();

    void tick();
};

#endif

