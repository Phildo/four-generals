#ifndef _FG_TURN_PICKER_H_
#define _FG_TURN_PICKER_H_

#include "turn.h"
#include "circ_q.h"
#include "ui.h"

class Graphics;
struct In;

class TurnPicker
{
  private:
    Turn t;
    circQ<Turn, 2> turn_q; //really should only ever be 1 in here.
    UI::Box box;

    bool active;
  public:
    TurnPicker();
    ~TurnPicker();

    void touch(In &in);
    bool query(In &in);
    void tick();
    void draw(Graphics *g);

    void activate();
    void clearViewState();

    bool getTurn(Turn &t);
    void reset();
};

#endif
