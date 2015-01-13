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
    void assignConTurn(char con, Turn t);
    void commitTurns();

    //clarity
    char& connectionConnection(char con);
    char& cardinalConnection(char card);
    VictoryRecord& connectionVictoryRecord(char con);
    VictoryRecord& cardinalVictoryRecord(char card);
    char connectionCardinal(char con);
    General& cardinalGeneral(char card);
    General& connectionGeneral(char con);
    Turn& generalTurn(General g);
    Turn& cardinalTurn(char card);
    Turn& connectionTurn(char con);
    Turn& generalDayTurn(General g, int day);
    Turn& cardinalDayTurn(char card, int day);
    Turn& connectionDayTurn(char con, int day);

    bool cardinalMessage( char card, Action& m);
    bool connectionMessage(char con, Action& m);
    bool cardinalIntruder( char card, Action& m0, Action& m1);
    bool connectionIntruder(char con, Action& m0, Action& m1);
    bool cardinalSabotage( char card, Action& m0, Action& m1);
    bool connectionSabotage(char con, Action& m0, Action& m1);

    bool cardinalConnected(char card);
    bool connectionConnected(char con);
    bool cardinalHasTurn(char card);
    bool connectionHasTurn(char con);

    bool cardinalWin(char card);
    bool connectionWin(char con);
    bool cardinalLose(char card);
    bool connectionLose(char con);
    bool cardinalTie(char card);
    bool connectionTie(char con);
    bool rolesAssigned();
    bool actionsAssigned();

    char currentDay();

    void zeroCurrentTurns();
    void zeroTomorrowsTurns();
    void zeroRound(); //leaves score/connections
    void zeroAll();

    Model();
    ~Model();
};

#endif

