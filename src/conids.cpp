#include "conids.h"

ConIds::ConIds()
{
  conids[0] = '1';
  conids[1] = '2';
  conids[2] = '3';
  conids[3] = '4';
  conids[4] = '0';
}

char ConIds::conid(int i)
{
  if(i > 3 || i < 0) return '0';
  return conids[i];
}

int ConIds::iconid(char c)
{
  switch(c)
  {
    case '1' : return 0;
    case '2' : return 1;
    case '3' : return 2;
    case '4' : return 3;
  }
  return 4;
}

bool ConIds::test()
{
  return
    conid(0) == '1' &&
    conid(3) == '4' &&
    conid(6) == '0' &&
    conid(10) == '0' &&
    conid(-1) == '0';
}

