#include "sabotage.h"

#include "logger.h"

#include <cstring> //memset for simple initialization

Sabotage::Sabotage()
{
  memset(this, '0', sizeof(Sabotage));
  id = 0;
}

Sabotage::Sabotage(Event e)
: cardinal(e.cardinal), how(e.how), which(e.which), what(e.what), who(e.who), when(e.when), where(e.where), id(e.sabotage_id)
{
}

Sabotage::Sabotage(char c, char ow, char wich, char wat, char wo, char wen, char were, int i)
: cardinal(c), how(ow), which(wich), what(wat), who(wo), when(wen), where(were), id(i)
{
}

Sabotage::~Sabotage()
{
}

