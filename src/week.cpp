#include "week.h"

Week::Week()
{
  days[0] = 's';
  days[1] = 'm';
  days[2] = 't';
  days[3] = 'w';
  days[4] = 'h';
  days[5] = 'f';
  days[6] = 'a';
  days[7] = '0';
}

char Week:day(int i)
{
  if(i > 6 || i < 0) return '0';
  return days[i];
}

int Week:iday(char c)
{
  switch(c)
  {
    case 's' : return 0;
    case 'm' : return 1;
    case 't' : return 2;
    case 'w' : return 3;
    case 'h' : return 4;
    case 'f' : return 5;
    case 'a' : return 6;
  }
  return 7;
}

char Week:nextday(char c)
{
  int i = iday(c); if(i == 7) return '0';
  return day((i+1)%7);
}

bool Week::test()
{
  return
    day(0) == 's' &&
    day(3) == 'w' &&
    day(6) == 'a' &&
    day(10) == '0' &&
    day(-1) == '0' &&
    nextday('s') == 'e' &&
    nextday('a') == 's' && //cycle
    nextday('0') == '0';
}

