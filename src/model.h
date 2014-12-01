#ifndef _FG_MODEL_H_
#define _FG_MODEL_H_

#include "general.h"
#include "messenger.h"
#include "sabotage.h"
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
    Event prev_actions[5]; //actions for prev turn

    //unordered
    Array<Messenger,16> messengers;
    Messenger nullMessenger; //equivalent to 5th member of other arrays

    //unordered
    Array<Sabotage,16> sabotages;
    Sabotage nullSabotage; //equivalent to 5th member of other arrays

    int days;
    char winning_card;
    char losing_card;

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
    Event& generalPrevAction(General g);
    Event& cardinalPrevAction(char card);
    Event& connectionPrevAction(char con);
    Messenger& cardinalMessage(char card);
    Messenger& connectionMessage(char con);
    Messenger& cardinalIntruder(char card);
    Messenger& connectionIntruder(char con);
    Sabotage& cardinalSabotage(char card);
    Sabotage& connectionSabotage(char con);
    bool cardinalConnected(char card);
    bool connectionConnected(char con);
    bool cardinalHasAction(char card);
    bool connectionHasAction(char con);
    bool cardinalHasMessage(char card);
    bool connectionHasMessage(char con);
    bool cardinalHasIntruder(char card);
    bool connectionHasIntruder(char con);
    bool cardinalWin(char card);
    bool connectionWin(char con);
    bool cardinalLose(char card);
    bool connectionLose(char con);
    bool rolesAssigned();
    bool actionsAssigned();

    char currentDay();

    void zeroActions();
    void zeroPrevActions();
    void zeroMessengers();
    void zeroSabotages();
    void zero(); //'resets' game

    Model();
    ~Model();
};

#endif

