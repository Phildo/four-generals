#ifndef _FG_SHOW_TURN_PICKER_H_
#define _FG_SHOW_TURN_PICKER_H_

#include "turn.h"
#include "ui.h"

class Graphics;
struct In;

struct ShowRequest
{
  enum SHOW_REQUEST_TYPE
  {
    NONE,
    ADD_ACTION,
    EDIT_ACTION,
    CONFIRM,
    COUNT
  } type;
  int action; //0 or 1
};

class ShowTurnPicker
{
  private:
    Turn *turn;

    UI::Label titleLabel;
    UI::Image power_0;
    UI::Image power_1;
    UI::ImageButtonRound action_0_1;
    UI::ImageButtonRound action_0_2;
    UI::ImageButtonRound action_1_2;
    UI::TextButton cancel;
    UI::TextButton confirm;

  public:
    UI::Box box;
    ShowTurnPicker();
    ShowTurnPicker(Turn * t, UI::Box wbox);
    ~ShowTurnPicker();

    ShowRequest touch(In &in);
    void tick();
    void draw(Graphics *g);

    void deactivate();
    void activate();
};

#endif

