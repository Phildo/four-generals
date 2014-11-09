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

char Model::conToCard(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return generals[i].cardinal;
  return '0';
}

char Model::cardToCon(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return generals[i].connection;
  return '0';
}

void Model::connectCon(char con)
{
  conConnection(con) = con;
}

void Model::disconnectCon(char con)
{
  conConnection(con) = '0';
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

char Model::connection(int i)
{
  if(i > 3 || i < 0) return '0';
  return connections[i];
}

int Model::iconnection(char c)
{
  switch(c)
  {
    case '1' : return 0;
    case '2' : return 1;
    case '3' : return 2;
    case '4' : return 3;
  }
  return 4;
}

//clarity

char& Model::conConnection(char con)
{
  return connections[iconnection(con)];
}

char& Model::cardinalConnection(char card)
{
  return connections[iconnection(cardinalGeneral(card).connection)];
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
  return cardinalAction(cardinalConnection(con));
}

bool Model::cardinalConnected(char card)
{
  return cardinalGeneral(card).connection != '0';
}

bool Model::connectionConnected(char con)
{
  return conConnection(con) != '0';
}

bool Model::rolesAssigned()
{
  return
    cardinalConnected('n') &&
    cardinalConnected('e') &&
    cardinalConnected('s') &&
    cardinalConnected('w');
}

Model::~Model()
{
}

