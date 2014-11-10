#ifndef _FG_WEEK_H_
#define _FG_WEEK_H_

namespace Week
{
  static const char days[8] = {'s','m','t','w','h','f','a','0'};
  char day(int i);
  int iday(char c);
  char nextday(char c);
  bool test();
}

#endif

