#ifndef _FG_COMPASS_H_
#define _FG_COMPASS_H_

namespace Compass
{
  static const char cardinals[5] = {'n','e','s','w','0'};
  char cardinal(int i);
  int icardinal(char c);
  char cwcardinal(char c);
  char ccwcardinal(char c);
  char opcardinal(char c);
  bool test();
}

#endif

