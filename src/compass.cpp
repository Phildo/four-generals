#include "compass.h"

char Compass::cardinal(int i)
{
  if(i > 3 || i < 0) return '0';
  return cardinals[i];
}

int Compass::icardinal(char c)
{
  switch(c)
  {
    case 'n': return 0;
    case 'e': return 1;
    case 's': return 2;
    case 'w': return 3;
  }
  return 4;
}

char Compass::cwcardinal(char c)
{
  int i = icardinal(c); if(i == 4) return '0';
  return cardinal((icardinal(c)+1)%4);
}

char Compass::ccwcardinal(char c)
{
  int i = icardinal(c); if(i == 4) return '0';
  return cardinal((icardinal(c)+3)%4); //+3%4 cross-language cycling (too lazy to check c's behavior)
}

char Compass::opcardinal(char c)
{
  int i = icardinal(c); if(i == 4) return '0';
  return cardinal((icardinal(c)+2)%4);
}

bool Compass::test()
{
  return
    cardinal(0) == 'n' &&
    cardinal(3) == 'w' &&
    cardinal(4) == '0' &&
    cardinal(10) == '0' &&
    cardinal(-1) == '0' &&
    cwcardinal('n') == 'e' &&
    cwcardinal('w') == 'n' && //cycle
    cwcardinal('0') == '0' &&
    ccwcardinal('s') == 'e' &&
    ccwcardinal('n') == 'w' && //cycle
    ccwcardinal('0') == '0' &&
    opcardinal('n') == 's' &&
    opcardinal('s') == 'n' &&
    opcardinal('e') == 'w' &&
    opcardinal('w') == 'e' &&
    opcardinal('0') == '0';
}

