#include "model.h"

#include "defines.h"
#include "logger.h"

Model::Model()
{
  connections[0] = '0';
  connections[1] = '0';
  connections[2] = '0';
  connections[3] = '0';
  connections[4] = '0';
  days = -1;
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
    char card = compass.cardinal(i);
    Event a = cardinalAction(card);
    if(a.action == 'a')
    {
      if(cardinalAction(a.who).action != 'd' && //attackee isn't defending
         cardinalAction(compass.opcardinal(card)).action == 'a' && //partner is attacking
         cardinalAction(compass.opcardinal(card)).who == a.who) //the same person as you
      {
        //win
      }
      else if(cardinalAction(a.who).action == 'd' && //attackee is defending
            !(cardinalAction(compass.opcardinal(card)).action == 'a' && //and partner isn't attacking
              cardinalAction(compass.opcardinal(card)).who == a.who)) //the same person as you
      {
        //lose
      }
    }
    else if(a.action == 'm')
    {
      //m = Messenger(*e);
      //messengers.add(m);
    }
  }

  //update messengers
  for(int i = 0; i < messengers.length(); i++)
  {
    if(messengers[i].at == messengers[i].to) //last
      messengers.remove(i); i--; //display message
    if(messengers[i].at == messengers[i].where) //middle
      messengers[i].at = messengers[i].to; //allow sabotage
    if(messengers[i].at == messengers[i].from) //first
      messengers[i].at = messengers[i].where;
  }

  cardinalAction('n').zero();
  cardinalAction('e').zero();
  cardinalAction('s').zero();
  cardinalAction('w').zero();

  days++;
}

//clarity

char& Model::connectionConnection(char con)
{
  return connections[conids.iconid(con)];
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
  return generals[compass.icardinal(card)];
}

General& Model::connectionGeneral(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return generals[i];
  return generals[4];
}

Event& Model::generalAction(General g)
{
  return actions[compass.icardinal(g.cardinal)];
}

Event& Model::cardinalAction(char card)
{
  return actions[compass.icardinal(card)];
}

Event& Model::connectionAction(char con)
{
  return cardinalAction(connectionCardinal(con));
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
  return week.day((days+7)%7); //+7 for obvious cross-lang modulus behavior
}

Model::~Model()
{
}

