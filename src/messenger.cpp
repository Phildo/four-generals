#include "messenger.h"

#include <cstring> //memset for simple initialization

Messenger::Messenger()
{
  memset(this, '0', sizeof(Messenger));
}

Messenger::Messenger(char fro, char a, char t, char wat, char wo, char wen, char were)
: from(fro), at(a), to(t), what(wat), who(wo), when(wen), where(were)
{
}

Messenger::~Messenger()
{
}

