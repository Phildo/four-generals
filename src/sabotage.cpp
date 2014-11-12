#include "sabotage.h"

#include "logger.h"

#include <cstring> //memset for simple initialization

Sabotage::Sabotage()
{
  memset(this, '0', sizeof(Sabotage));
  id = 0;
}

Sabotage::Sabotage(Event e)
: how(e.how), which(e.which), what(e.what), who(e.who), when(e.when), where(e.where), id(e.sabotage_id_i)
{
}

Sabotage::Sabotage(char h, char i, char wat, char wo, char wen, char were, int i);
: how(h), which(i), what(wat), who(wo), when(wen), where(were), id(i)
{
}

Sabotage::~Sabotage()
{
}

