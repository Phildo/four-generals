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
  days = -1;
  winning_card = '0';
  losing_card = '0';
}

void Model::connectCon(char con)
{
  connectionConnection(con) = con;
}

void Model::disconnectCon(char con)
{
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
    else if(a.action == 'm')
    {
      Messenger m(a);
      messengers.add(m);
    }
    else if(a.action == 's')
    {
      Sabotage s(a);
      sabotages.add(s);
    }
  }

  //update messengers
  for(int i = 0; i < messengers.length(); i++)
    if(!messengers[i].advance()) { messengers.remove(i); i--; }

  //apply sabotages
  for(int i = 0; i < sabotages.length(); i++)
  {
    if(cardinalHasIntruder(sabotages[i].cardinal) && !(cardinalIntruder(sabotages[i].cardinal).sabotage(sabotages[i])))
      messengers.remove(cardinalIntruder(sabotages[i].cardinal));
  }
  sabotages.removeAll();

  cardinalAction('n').zero();
  cardinalAction('e').zero();
  cardinalAction('s').zero();
  cardinalAction('w').zero();

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
  return actions[Compass::icardinal(g.cardinal)];
}

Event& Model::cardinalAction(char card)
{
  return actions[Compass::icardinal(card)];
}

Event& Model::connectionAction(char con)
{
  return cardinalAction(connectionCardinal(con));
}

Messenger& Model::cardinalMessage(char card)
{
  for(int i = 0; i < messengers.length(); i++)
    if(messengers[i].to == card &&
       messengers[i].at == card) return messengers[i];
  return nullMessenger;
}

Messenger& Model::connectionMessage(char con)
{
  return cardinalMessage(connectionCardinal(con));
}

Messenger& Model::cardinalIntruder(char card)
{
  for(int i = 0; i < messengers.length(); i++)
    if(messengers[i].from != card &&
       messengers[i].to   != card &&
       messengers[i].at   == card) return messengers[i];
  return nullMessenger;
}

Messenger& Model::connectionIntruder(char con)
{
  return cardinalIntruder(connectionCardinal(con));
}

Sabotage& Model::cardinalSabotage(char card)
{
  for(int i = 0; i < sabotages.length(); i++)
    if(sabotages[i].cardinal == card) return sabotages[i];
  return nullSabotage;
}

Sabotage& Model::connectionSabotage(char con)
{
  return cardinalSabotage(connectionCardinal(con));
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
  for(int i = 0; i < messengers.length(); i++)
    if(messengers[i].to == card &&
       messengers[i].at == card) return true;
  return false;
}

bool Model::connectionHasMessage(char con)
{
  return cardinalHasMessage(connectionCardinal(con));
}

bool Model::cardinalHasIntruder(char card)
{
  for(int i = 0; i < messengers.length(); i++)
    if(messengers[i].from != card &&
       messengers[i].to   != card &&
       messengers[i].at   == card) return true;
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

Model::~Model()
{
}

