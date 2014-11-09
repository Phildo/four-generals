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

    //in connection order
    char connections[5]; //connections[0] should be '1' for here, '0' for not

    //in cardinal order
    General generals[5];
    Event actions[5]; //actions for this turn

    //unordered
    Array<Messenger,16> messengers;

    int days;

    char connection(int i);
    int iconnection(char c);

    char conToCard(char con);
    char cardToCon(char card);

    void connectCon(char con);
    void disconnectCon(char con);
    void assignConCard(char con, char card);
    void revokeCard(char card);

    //clarity
    char& conConnection(char con);
    char& cardinalConnection(char card);
    General& cardinalGeneral(char card);
    General& connectionGeneral(char con);
    Event& generalAction(General g);
    Event& cardinalAction(char card);
    Event& connectionAction(char con);
    bool cardinalConnected(char card);
    bool connectionConnected(char con);
    bool rolesAssigned();

    Model();
    ~Model();
};

#endif

