#ifndef _FG_SPECIFY_TURN_PICKER_H_
#define _FG_SPECIFY_TURN_PICKER_H_

#include "turn.h"
#include "ui.h"

class Graphics;
struct In;

struct SpecifyRequest
{
  enum SHOW_REQUEST_TYPE
  {
    NONE,
    CONFIRM_ACTION,
    CANCEL_SPECIFY,
    COUNT
  } type;
  int action; //0 or 1
};

class SpecifyTurnPicker
{
  private:
    Turn *turn;
    Action *action;

    UI::Label titleLabel;
    UI::Image power_0;
    UI::Image power_1;
    UI::ImageButtonRound add;
    UI::ImageButtonRound action_0;
    UI::ImageButtonRound action_1;
    UI::TextButton cancel;
    UI::TextButton confirm;

  public:
    UI::Box box;
    SpecifyTurnPicker();
    SpecifyTurnPicker(Turn *t, UI::Box wbox);
    ~SpecifyTurnPicker();

    SpecifyRequest touch(In &in);
    void tick();
    void draw(Graphics *g);

    void setAction(Action *a);
    void deactivate();
    void activate();
};

#endif

