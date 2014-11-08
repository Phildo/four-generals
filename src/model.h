#ifndef _FG_MODEL_H_
#define _FG_MODEL_H_

#include "compass.h"
#include "week.h"

#include "general.h"
#include "messenger.h"
#include "event.h"

#include "array.h"

class Model
{
  private :
  public :
    Compass compass;
    Week week;

    char connections[5]; //connections[0] should be '1' for here, '0' for not
    General generals[4];
    Event actions[4]; //actions for this turn
    Array<Messenger,16> messengers;

    char conToCard(char con);
    char cardToCon(char card);

    int days;

    Model();
    ~Model();
};

#endif

