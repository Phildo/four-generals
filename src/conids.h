#ifndef _FG_CON_IDS_H_
#define _FG_CON_IDS_H_

class ConIds
{
  private:
    char conids[5];
  public:
    ConIds();
    char conid(int i);
    int iconid(char c);
    bool test();
};

#endif

