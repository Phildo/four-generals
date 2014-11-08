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
    if(generals[i].connection == con) return generals[i].card;
}

char Model::cardToCon(char card)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].cardinal == card) return generals[i].connection;
}

char Connection:connection(int i)
{
  if(i > 3 || i < 0) return '0';
  return connection[i];
}

int Connection:iconnection(char c)
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

Model::~Model()
{
}

