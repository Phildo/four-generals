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
  zero();
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

void Model::assignConAction(char con, Event e)
{
  connectionAction(con) = e; //copy
}

void Model::commitActions()
{
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);
    Event a = cardinalAction(card);
    if(a.action == 'a')
    {
      if(cardinalAction(a.who).action != 'd' && //attackee isn't defending
         cardinalAction(Compass::opcardinal(card)).action == 'a' && //partner is attacking
         cardinalAction(Compass::opcardinal(card)).who == a.who) //the same person as you
      {
        winning_card = card;
        losing_card = a.who;
      }
      else if(cardinalAction(a.who).action == 'd' && //attackee is defending
            !(cardinalAction(Compass::opcardinal(card)).action == 'a' && //and partner isn't attacking
              cardinalAction(Compass::opcardinal(card)).who == a.who)) //the same person as you
      {
        losing_card = card;
        winning_card = a.who;
      }
    }
    else if(a.action == 'm') { }
    else if(a.action == 's') { }
  }

  zeroTomorrowsActions();
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

Event& Model::generalAction(General g)
{
  return generalDayAction(g, days);
}

Event& Model::cardinalAction(char card)
{
  return cardinalDayAction(card, days);
}

Event& Model::connectionAction(char con)
{
  return connectionDayAction(con, days);
}

Event& Model::generalDayAction(General g, int day)
{
  if(day < 0 || day >= FG_MAX_ACTION_HIST || day > days) return actions[FG_MAX_ACTION_HIST*4];
  return actions[(day*4)+Compass::icardinal(g.cardinal)];
}

Event& Model::cardinalDayAction(char card, int day)
{
  if(day < 0 || day >= FG_MAX_ACTION_HIST || day > days) return actions[FG_MAX_ACTION_HIST*4];
  return actions[(day*4)+Compass::icardinal(card)];
}

Event& Model::connectionDayAction(char con, int day)
{
  return cardinalDayAction(connectionCardinal(con), day);
}


Messenger Model::cardinalMessage(char card)
{
  Messenger m;
  Messenger n;
  if(days < 2) return n;
  char p = Compass::opcardinal(card);
  Event me = cardinalDayAction(p, days-2);
  if(me.action != 'm') return n;
  m = Messenger(me);
  Event se = cardinalDayAction(me.where, days-2);
  if(se.action != 's') return m;
  m.sabotage(Sabotage(se));
  if(m.sabotaged == 'b') return n; //blocked
  return m;
}

Messenger Model::connectionMessage(char con)
{
  return cardinalMessage(connectionCardinal(con));
}

Messenger Model::cardinalIntruder(char card)
{
  Messenger m;
  Messenger n;
  char e;
  Event me;
  if(days < 1) return n;

  e = Compass::cwcardinal(card);
  me = cardinalDayAction(e, days-1);
  if(me.action == 'm')
  {
    m = Messenger(me);
    return m;
  }

  e = Compass::ccwcardinal(card);
  me = cardinalDayAction(e, days-1);
  if(me.action == 'm')
  {
    m = Messenger(me);
    return m;
  }

  return n;
}

Messenger Model::connectionIntruder(char con)
{
  return cardinalIntruder(connectionCardinal(con));
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

bool Model::cardinalHasMessage(char card)
{
  Messenger m = cardinalMessage(card);
  if(m.id) return true;
  return false;
}

bool Model::connectionHasMessage(char con)
{
  return cardinalHasMessage(connectionCardinal(con));
}

bool Model::cardinalHasIntruder(char card)
{
  Messenger m = cardinalIntruder(card);
  if(m.id) return true;
  return false;
}

bool Model::connectionHasIntruder(char con)
{
  return cardinalHasIntruder(connectionCardinal(con));
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

void Model::zeroCurrentActions()
{
  if(days < 0 || days >= FG_MAX_ACTION_HIST) return;
  for(int i = 0; i < 5; i++)
    actions[(days*4)+i].zero();
}

void Model::zeroTomorrowsActions()
{
  if(days < -1 || days >= (FG_MAX_ACTION_HIST-1)) return;
  for(int i = 0; i < 5; i++)
    actions[((days+1)*4)+i].zero();
}

void Model::zero() //'resets' gam
{
  days = -1;
  zeroTomorrowsActions();
  winning_card = '0';
  losing_card = '0';
}

Model::~Model()
{
}

