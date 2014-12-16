#ifndef _FG_MODEL_H_
#define _FG_MODEL_H_

#include "general.h"
#include "messenger.h"
#include "sabotage.h"
#include "event.h"

#include "array.h"

#define FG_MAX_ACTION_HIST 32

class Model
{
  private :
  public :
    //in connection order
    char connections[5];

    //in cardinal order
    General generals[5];
    Event actions[4*FG_MAX_ACTION_HIST+1]; //2d array [(day*4)+general]

    int days;
    char winning_card;
    char losing_card;
    char tieing_card;

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
    Event& generalDayAction(General g, int day);
    Event& cardinalDayAction(char card, int day);
    Event& connectionDayAction(char con, int day);
    Messenger cardinalMessage(char card);
    Messenger connectionMessage(char con);
    Messenger cardinalIntruder(char card);
    Messenger connectionIntruder(char con);
    Messenger cardinalSabotage(char card);
    Messenger connectionSabotage(char con);
    bool cardinalConnected(char card);
    bool connectionConnected(char con);
    bool cardinalHasAction(char card);
    bool connectionHasAction(char con);
    bool cardinalHasMessage(char card);
    bool connectionHasMessage(char con);
    bool cardinalHasIntruder(char card);
    bool connectionHasIntruder(char con);
    bool cardinalHasSabotage(char card);
    bool connectionHasSabotage(char con);
    bool cardinalWin(char card);
    bool connectionWin(char con);
    bool cardinalLose(char card);
    bool connectionLose(char con);
    bool cardinalTie(char card);
    bool connectionTie(char con);
    bool rolesAssigned();
    bool actionsAssigned();

    char currentDay();

    void zeroCurrentActions();
    void zeroTomorrowsActions();
    void zero(); //'resets' game

    Model();
    ~Model();
};

#endif

