#ifndef _FG_MODEL_H_
#define _FG_MODEL_H_

#include "compass.h"
#include "week.h"
#include "conids.h"

#include "general.h"
#include "messenger.h"
#include "event.h"

#include "array.h"

class Model
{
  private :
  public :
    //in connection order
    char connections[5];

    //in cardinal order
    General generals[5];
    Event actions[5]; //actions for this turn

    //unordered
    Array<Messenger,16> messengers;

    int days;

    void connectCon(char con);
    void disconnectCon(char con);
    void assignConCard(char con, char card);
    void revokeCard(char card);
    void assignConAction(char con, Event e);
    void commitActions();

    //clarity
    char& connectionConnection(char con);
    char& cardinalConnection(char card);
    char connectionCardinal(char con);
    General& cardinalGeneral(char card);
    General& connectionGeneral(char con);
    Event& generalAction(General g);
    Event& cardinalAction(char card);
    Event& connectionAction(char con);
    bool cardinalConnected(char card);
    bool connectionConnected(char con);
    bool cardinalHasAction(char card);
    bool connectionHasAction(char con);
    bool rolesAssigned();
    bool actionsAssigned();

    char currentDay();

    Model();
    ~Model();
};

#endif

