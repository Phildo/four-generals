#include "model.h"
#include "defines.h"
#include "logger.h"
#include <stdlib.h>

Model::Model()
{
  generals[0].cardinal = 'n';
  generals[1].cardinal = 'w';
  generals[2].cardinal = 's';
  generals[3].cardinal = 'e';
}

Model::~Model()
{
}

