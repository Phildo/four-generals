#ifndef _FG_MESSENGER_H_
#define _FG_MESSENGER_H_

class Messenger
{
  private:
  public:
    char cardinal_from; //cardinal_to derived as opposite from
    char cardinal_at;

    char message_instruction; //'a'ttack, 'd'efend
    char message_day; //smtwhfa
    char message_cardinal;

    Messenger();
    ~Messenger();
};

#endif

