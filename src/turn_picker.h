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
    UI::Box win_box;

    UI::Box box;
    //views
    UI::Label titleLabel;
    UI::Image power_0;
    UI::Image power_1;

    UI::ImageButtonRound add;

    UI::TextButton cancel;
    UI::TextButton confirm;

    enum TURN_PICKER_STATE //the visual state of the view
    {
      IDLE,         // can choose to look at messages or pick turn
      MESSAGE,      // full screen message/sabotage showing
      TURN_PICKING, // full screen action picker
      WAITING,      // picked turn, waiting on other players
      SHOWING,      // animating newly played out action (interaction disabled)
      OVER,         // game over
      COUNT
    } state;
    void setViewState(TURN_PICKER_STATE s); //clears state vars and sets state

    void init();
  public:
    TurnPicker();
    TurnPicker(UI::Box wbox);
    ~TurnPicker();

    void touch(In &in);
    bool query(In &in);
    void tick();
    void draw(Graphics *g);

    void clearViewState();

    bool getTurn(Turn &t);
    void reset();
};

#endif

