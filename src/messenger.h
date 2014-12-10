#ifndef _FG_MESSENGER_H_
#define _FG_MESSENGER_H_

#include "event.h"
#include "sabotage.h"

class Messenger
{
  private:
  public:
    //location
    char from;
    char was;
    char at;
    char to;

    char what;  //'a' = attack, 'd' = defend, 's' = sabotage
    char who;   //cardinal (about whom the contents of the message apply)
    char when;  //'s|m|t|w|h|f|a'
    char where; //cardinal (through which route messenger will take)

    int id;
    char sabotaged;

    Messenger();
    Messenger(Event e);
    Messenger(char fro, char a, char t, char wat, char wo, char wen, char were, int i);

    bool operator==(Messenger m);

    bool advance();
    void sabotage(Sabotage s);

    char *message();
    char message_buff[256];

    ~Messenger();
};

#endif

