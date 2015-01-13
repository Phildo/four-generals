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
  revokeCard(connectionGeneral(con).cardinal);
  connectionConnection(con) = '0';
}

void Model::assignConCard(char con, char card)
{
  revokeCard(connectionGeneral(con).cardinal);
  cardinalGeneral(card).connection = con;
  cardinalGeneral(card).cardinal   = card;
}

void Model::revokeCard(char card)
{
  cardinalGeneral(card).connection = '0'; //set connection first,
  cardinalGeneral(card).cardinal   = '0'; //else won't be able to find it here
}

void Model::assignConTurn(char con, Event e)
{
  connectionTurn(con) = e; //copy
}

void Model::commitTurns()
{
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);
    Event a = cardinalTurn(card);
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
    char con = ConIds::conid(i);
    if(connectionWin(con))  connectionVictoryRecord(con).win++;
    if(connectionLose(con)) connectionVictoryRecord(con).loss++;
    if(connectionTie(con))  connectionVictoryRecord(con).tie++;
  }

  zeroTomorrowsTurns();
  days++;
}

//clarity

char& Model::connectionConnection(char con)
{
  return connections[ConIds::iconid(con)];
}

char& Model::cardinalConnection(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return connectionConnection(generals[i].connection);
  return connectionConnection('0');
}

VictoryRecord& Model::connectionVictoryRecord(char con)
{
  return connectionRecords[ConIds::iconid(con)];
}

VictoryRecord& Model::cardinalVictoryRecord(char card)
{
  return connectionVictoryRecord(cardinalConnection(card));
}

char Model::connectionCardinal(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return generals[i].cardinal;
  return '0';
}

General& Model::cardinalGeneral(char card)
{
  return generals[Compass::icardinal(card)];
}

General& Model::connectionGeneral(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return generals[i];
  return generals[4];
}

Event& Model::generalTurn(General g)
{
  return generalDayTurn(g, days);
}

Event& Model::cardinalTurn(char card)
{
  return cardinalDayTurn(card, days);
}

Event& Model::connectionTurn(char con)
{
  return connectionDayTurn(con, days);
}

Event& Model::generalDayTurn(General g, int day)
{
  if(day < 0 || day >= FG_MAX_ACTION_HIST || day > days) return actions[FG_MAX_ACTION_HIST*4];
  return actions[(day*4)+Compass::icardinal(g.cardinal)];
}

Event& Model::cardinalDayTurn(char card, int day)
{
  if(day < 0 || day >= FG_MAX_ACTION_HIST || day > days) return actions[FG_MAX_ACTION_HIST*4];
  return actions[(day*4)+Compass::icardinal(card)];
}

Event& Model::connectionDayTurn(char con, int day)
{
  return cardinalDayTurn(connectionCardinal(con), day);
}

bool Model::cardinalMessage(char card, Turn& m)
{
  if(days < 2) return false;

  char p = Compass::opcardinal(card);

  Event me = cardinalDayTurn(p, days-2);
  if(me.action != 'm') return false;
  m = Turn(me);

  Event se = cardinalDayTurn(me.route, days-2);
  if(se.action != 's') return true;
  if(se.how == 'b') return false;
  m.sabotage(Sabotage(se));

  return true;
}

bool Model::connectionMessage(char con, Turn& m)
{
  return cardinalMessage(connectionCardinal(con), m);
}

bool Model::cardinalIntruder(char card, Turn& m0, Turn& m1)
{
  Turn m;
  char e;
  Event me;
  m0.zero();
  m1.zero();
  if(days < 1) return false;

  e = Compass::cwcardinal(card);
  me = cardinalDayTurn(e, days-1);
  if(me.action == 'm')
  {
    m = Turn(me);
    if(m.route == card)
    {
      m0 = m;
    }
  }

  e = Compass::ccwcardinal(card);
  me = cardinalDayTurn(e, days-1);
  if(me.action == 'm')
  {
    m = Turn(me);
    if(m.route == card)
    {
      if(!m0.id) m0 = m;
      else m1 = m;
    }
  }

  if(m0.id) return true;

  return false;
}

bool Model::connectionIntruder(char con, Action& m0, Action& m1)
{
  return cardinalIntruder(connectionCardinal(con), m0, m1);
}

bool Model::cardinalSabotage(char card, Action& m0, Action& m1)
{
  Event e = cardinalDayAction(card, days-1);
  if(e.action != 's' || e.how != 'r')
  {
    m0.zero();
    m1.zero();
    return false;
  }
  else return cardinalIntruder(card, m0, m1);
}

bool Model::connectionSabotage(char con, Action& m0, Action& m1)
{
  return cardinalSabotage(connectionCardinal(con), m0, m1);
}

bool Model::cardinalConnected(char card)
{
  return cardinalGeneral(card).connection != '0';
}

bool Model::connectionConnected(char con)
{
  return connectionConnection(con) != '0';
}

bool Model::cardinalHasAction(char card)
{
  return cardinalAction(card).type != '0';
}

bool Model::connectionHasAction(char con)
{
  return connectionAction(con).type != '0';
}

bool Model::cardinalWin(char card)
{
  return (card == winning_card || Compass::opcardinal(card) == winning_card);
}

bool Model::connectionWin(char con)
{
  return cardinalWin(connectionCardinal(con));
}

bool Model::cardinalLose(char card)
{
  return (card == losing_card || Compass::opcardinal(card) == losing_card);
}

bool Model::connectionLose(char con)
{
  return cardinalLose(connectionCardinal(con));
}

bool Model::cardinalTie(char card)
{
  return tieing_card != '0';
}

bool Model::connectionTie(char con)
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

bool Model::actionsAssigned()
{
  return
    cardinalHasAction('n') &&
    cardinalHasAction('e') &&
    cardinalHasAction('s') &&
    cardinalHasAction('w');
}

char Model::currentDay()
{
  return Week::day((days+7)%7); //+7 for obvious cross-lang modulus behavior
}

void Model::zeroCurrentTurns()
{
  if(days < 0 || days >= FG_MAX_ACTION_HIST) return;
  for(int i = 0; i < 5; i++)
    actions[(days*4)+i].zero();
}

void Model::zeroTomorrowsTurns()
{
  if(days < -1 || days >= (FG_MAX_ACTION_HIST-1)) return;
  for(int i = 0; i < 5; i++)
    actions[((days+1)*4)+i].zero();
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

