#ifndef _FG_MESSENGER_H_
#define _FG_MESSENGER_H_

class Messenger
{
  private:
  public:
    //location
    char from;
    char at;
    char to;

    char what;  //'a' = attack, 'd' = defend, 's' = sabotage
    char who;   //cardinal (about whom the contents of the message apply)
    char when;  //'s|m|t|w|h|f|a'
    char where; //cardinal (through which route messenger will take)

    Messenger();
    Messenger(char fro, char a, char t, char wat, char wo, char wen, char were);

    ~Messenger();
};

#endif

