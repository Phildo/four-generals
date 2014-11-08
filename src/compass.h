#ifndef _FG_COMPASS_H_
#define _FG_COMPASS_H_

//stupid operations on a direction
class Compass
{
  private:
    char cardinals[5];
  public:
    Compass();
    char cardinal(int i);
    int icardinal(char c);
    char cwcardinal(char c);
    char ccwcardinal(char c);
    char opcardinal(char c);

    bool test();
};

#endif

