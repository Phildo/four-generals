#ifndef _FG_BROWSE_TURN_PICKER_H_
#define _FG_BROWSE_TURN_PICKER_H_

#include "turn.h"
#include "ui.h"

class Graphics;
struct In;

class BrowseTurnPicker
{
  private:
    UI::Box box;
    UI::Label titleLabel;
    UI::Image power_0;
    UI::Image power_1;
    UI::ImageButtonRound add;
    UI::ImageButtonRound action_0;
    UI::ImageButtonRound action_1;
    UI::TextButton cancel;
    UI::TextButton confirm;

  public:
    BrowseTurnPicker();
    BrowseTurnPicker(UI::Box wbox);
    ~BrowseTurnPicker();

    void touch(In &in);
    void tick();
    void draw(Turn t, Graphics *g);
};

#endif

