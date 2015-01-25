#ifndef _FG_TURN_PICKER_H_
#define _FG_TURN_PICKER_H_

#include "turn.h"
#include "circ_q.h"
#include "ui.h"

#include "show_turn_picker.h"
#include "browse_turn_picker.h"
#include "specify_turn_picker.h"

class Graphics;
struct In;

class TurnPicker
{
  private:
    Turn turn;
    circQ<Turn, 2> turn_q; //really should only ever be 1 in here.
    UI::Box win_box;

    //views
    UI::Box box;
    UI::Button button;

    ShowTurnPicker show;
    BrowseTurnPicker browse;
    SpecifyTurnPicker specify;

    enum TURN_PICKER_STATE //the visual state of the view
    {
      SHOW,    // shows currently chosen actions (or + if none chosen)
      BROWSE,  // browse scrollable list of choosable actions
      SPECIFY, // action chosen, choosing details
      COUNT
    } state;
    void setViewState(TURN_PICKER_STATE s); //clears state vars and sets state
  public:
    TurnPicker();
    TurnPicker(UI::Box wbox);
    ~TurnPicker();

    void init();

    void touch(In &in);
    bool query(In &in);
    void tick();
    void draw(Graphics *g);

    void clearViewState();

    bool getTurn(Turn &t);
    void reset();
};

#endif

