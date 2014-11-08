#ifndef _FG_WEEK_H_
#define _FG_WEEK_H_

class Week
{
  private:
    char days[8];
  public:
    Week();
    char day(int i);
    int iday(char c);
    char nextday(char c);
    bool test();
};

#endif

