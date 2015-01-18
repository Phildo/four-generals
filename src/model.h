#ifndef _FG_MODEL_H_
#define _FG_MODEL_H_

#include "general.h"
#include "turn.h"
#include "victory_record.h"

#include "array.h"

#define FG_MAX_ACTION_HIST 32

class Model
{
  private :
  public :
    //in connection order
    char connections[5];
    VictoryRecord connectionRecords[5]; //why this is a separate struct... who knows.

    //in cardinal order
    General generals[5];
    Turn turns[4*FG_MAX_ACTION_HIST+1]; //2d array [(day*4)+general]

    int days;
    char winning_card;
    char losing_card;
    char tieing_card;

    void connectCon(char con);
    void disconnectCon(char con);
    void assignConCard(char con, char card);
    void revokeCard(char card);
    void assignCardinalTurn(char card, Turn t);
    void commitTurns();

    //routers
    char& connectionConnection(char con);
    VictoryRecord& connectionVictoryRecord(char con);
    char connectionCardinal(char con);
    bool connectionConnected(char con);

    char& cardinalConnection(char card);
    General& cardinalGeneral(char card);
    Turn& cardinalTurn(char card);
    Turn& cardinalDayTurn(char card, int day);
    bool cardinalMessage(  char card, Action& a);
    bool cardinalIntruder( char card, Action& a0, Action& a1);
    bool cardinalSabotage( char card, Action& a0, Action& a1);
    bool cardinalConnected(char card);
    bool cardinalHasTurn(char card);
    bool cardinalWin(char card);
    bool cardinalLose(char card);
    bool cardinalTie(char card);

    bool rolesAssigned();
    bool turnsAssigned();

    char currentDay();

    void zeroCurrentTurns();
    void zeroTomorrowsTurns();
    void zeroRound(); //leaves score/connections
    void zeroAll();

    Model();
    ~Model();
};

#endif

