#include "model.h"

#include "defines.h"
#include "logger.h"

#include "compass.h"
#include "week.h"
#include "conids.h"

Model::Model()
{
  connections[0] = '0';
  connections[1] = '0';
  connections[2] = '0';
  connections[3] = '0';
  connections[4] = '0';
  zeroAll();
}

void Model::connectCon(char con)
{
  connectionConnection(con) = con;
}

void Model::disconnectCon(char con)
{
  revokeCard(connectionCardinal(con));
  connectionConnection(con) = '0';
}

void Model::assignConCard(char con, char card)
{
  revokeCard(connectionCardinal(con));
  cardinalGeneral(card).connection = con;
  cardinalGeneral(card).cardinal   = card;
}

void Model::revokeCard(char card)
{
  cardinalGeneral(card).connection = '0';
  cardinalGeneral(card).cardinal   = '0';
}

void Model::assignCardinalTurn(char card, Turn t)
{
  cardinalTurn(card) = t;
}

void Model::commitTurns()
{
//dis gon b gud
/*
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);
    Turn t = cardinalTurn(card);
    if(a.action == 'a')
    {
      if(cardinalTurn(a.who).action != 'd' && //attackee isn't defending
         cardinalTurn(Compass::opcardinal(card)).action == 'a' && //partner is attacking
         cardinalTurn(Compass::opcardinal(card)).who == a.who) //the same person as you
      {
        if(cardinalTie(card) || cardinalLose(card))
        {
          winning_card = '0';
          losing_card = '0';
          tieing_card = card; //any non-zero card = everyone ties
        }
        else
        {
          winning_card = card;
          losing_card = a.who;
        }
      }
      else if(cardinalTurn(a.who).action == 'd' && //attackee is defending
            !(cardinalTurn(Compass::opcardinal(card)).action == 'a' && //and partner isn't attacking
              cardinalTurn(Compass::opcardinal(card)).who == a.who)) //the same person as you
      {
        if(cardinalTie(card) || cardinalWin(card))
        {
          winning_card = '0';
          losing_card = '0';
          tieing_card = card;
        }
        else
        {
          losing_card = card;
          winning_card = a.who;
        }
      }
    }
    else if(a.action == 'm') { }
    else if(a.action == 's') { }
  }

  for(int i = 0; i < 4; i++) //ascribe any victory points
  {
    char card = Compass::cardinal(i);
    if(cardinalWin(card))  connectionVictoryRecord(cardinalConnection(card)).win++;
    if(cardinalLose(card)) connectionVictoryRecord(cardinalConnection(card)).loss++;
    if(cardinalTie(card))  connectionVictoryRecord(cardinalConnection(card)).tie++;
  }

  zeroTomorrowsTurns();
  days++;
*/
}


//routers

char& Model::connectionConnection(char con)
{
  return connections[ConIds::iconid(con)];
}
VictoryRecord& Model::connectionVictoryRecord(char con)
{
  return connectionRecords[ConIds::iconid(con)];
}
char Model::connectionCardinal(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return generals[i].cardinal;
  return '0';
}


char& Model::cardinalConnection(char card)
{
  return connectionConnection(cardinalGeneral(card).connection);
}
General& Model::cardinalGeneral(char card)
{
  return generals[Compass::icardinal(card)];
}
Turn& Model::cardinalTurn(char card)
{
  return cardinalDayTurn(card, days);
}
Turn& Model::cardinalDayTurn(char card, int day)
{
  if(day < 0 || day >= FG_MAX_ACTION_HIST || day > days) return turns[FG_MAX_ACTION_HIST*4];
  return turns[(day*4)+Compass::icardinal(card)];
}

bool Model::cardinalMessage(char card, Action& a)
{
  a.zero();
  if(days < 2) return false;

  char p = Compass::opcardinal(card);

  //if partner sent message
  Turn t = cardinalDayTurn(p, days-2);
       if(t.actions[0].what == 'm') a = t.actions[0];
  else if(t.actions[1].what == 'm') a = t.actions[1];
  else                              return false;

  //if route was sabotaged
  t = cardinalDayTurn(a.route, days-2);
       if(t.actions[0].what == 's') a.beSabotaged(t.actions[0]);
  else if(t.actions[1].what == 's') a.beSabotaged(t.actions[1]);

  //if sabotage was block
  if((t.actions[0].what == 's' && t.actions[0].how == 'b') &&
     (t.actions[0].what == 's' && t.actions[0].how == 'b'))
  {
    a.zero();
    return false;
  }

  return true;
}

bool Model::cardinalIntruder(char card, Action& a0, Action& a1)
{
  a0.zero();
  a1.zero();
  if(days < 1) return false;

  char e;
  Turn t;

  e = Compass::cwcardinal(card);
  t = cardinalDayTurn(e, days-1);
  if(t.actions[0].what == 'm' && t.actions[0].route == card)
      a0 = t.actions[0];
  if(t.actions[1].what == 'm' && t.actions[1].route == card)
      a0 = t.actions[1];

  e = Compass::ccwcardinal(card);
  t = cardinalDayTurn(e, days-1);
  if(t.actions[0].what == 'm' && t.actions[0].route == card)
  {
    if(a0.what == '0') a0 = t.actions[0];
    else               a1 = t.actions[0];
  }
  if(t.actions[1].what == 'm' && t.actions[1].route == card)
  {
    if(a0.what == '0') a0 = t.actions[1];
    else               a1 = t.actions[1];
  }

  return (a0.what != '0');
}

bool Model::cardinalSabotage(char card, Action& a0, Action& a1)
{
  a0.zero();
  a1.zero();

  Turn t = cardinalDayTurn(card, days-1);
  Action a;
       if(t.actions[0].what == 's') a = t.actions[0];
  else if(t.actions[1].what == 's') a = t.actions[1];
  else                              return false;

  if(a.how != 'r') return false;
  else return cardinalIntruder(card, a0, a1);
}

bool Model::cardinalConnected(char card)
{
  return cardinalGeneral(card).connection != '0';
}

bool Model::connectionConnected(char con)
{
  return connectionConnection(con) != '0';
}

bool Model::cardinalHasTurn(char card)
{
  return cardinalTurn(card).cardinal != '0';
}

bool Model::cardinalWin(char card)
{
  return (card == winning_card || Compass::opcardinal(card) == winning_card);
}

bool Model::cardinalLose(char card)
{
  return (card == losing_card || Compass::opcardinal(card) == losing_card);
}

bool Model::cardinalTie(char card)
{
  return tieing_card != '0';
}

bool Model::rolesAssigned()
{
  return
    cardinalConnected('n') &&
    cardinalConnected('e') &&
    cardinalConnected('s') &&
    cardinalConnected('w');
}

bool Model::turnsAssigned()
{
  return
    cardinalHasTurn('n') &&
    cardinalHasTurn('e') &&
    cardinalHasTurn('s') &&
    cardinalHasTurn('w');
}

char Model::currentDay()
{
  return Week::day((days+7)%7); //+7 for obvious cross-lang modulus behavior
}

void Model::zeroCurrentTurns()
{
  if(days < 0 || days >= FG_MAX_ACTION_HIST) return;
  for(int i = 0; i < 5; i++)
    turns[(days*4)+i].zero();
}

void Model::zeroTomorrowsTurns()
{
  if(days < -1 || days >= (FG_MAX_ACTION_HIST-1)) return;
  for(int i = 0; i < 5; i++)
    turns[((days+1)*4)+i].zero();
}

void Model::zeroRound()
{
  days = -1; //set to "not yet a valid day"
  zeroTomorrowsTurns();
  winning_card = '0';
  losing_card = '0';
  tieing_card = '0';
}

void Model::zeroAll()
{
  zeroRound();
  //don't actually know how to handle this yet...
}


Model::~Model()
{
}

