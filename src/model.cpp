#include "model.h"
#include "defines.h"
#include "logger.h"
#include <stdlib.h>

Model::Model(Network::Client *c)
{
  generals[0].cardinal = 'n';
  generals[1].cardinal = 'w';
  generals[2].cardinal = 's';
  generals[3].cardinal = 'e';

  client = c;
}

Model::~Model()
{
}

void Model::tick()
{
  Network::Event *e;
  while((e = client->getEvent()))
  {
    fg_log("Model sees %s",e->serialize());
  }

}

