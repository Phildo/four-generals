#ifndef _FG_VICTORY_RECORD_H_
#define _FG_VICTORY_RECORD_H_

struct VictoryRecord
{
  private:
  public:
    int win;
    int loss;
    int tie;

    void reset();

    VictoryRecord();
    ~VictoryRecord();
};

#endif

