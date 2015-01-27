#ifndef _FG_SPECIFY_TURN_PICKER_H_
#define _FG_SPECIFY_TURN_PICKER_H_

#include "turn.h"
#include "ui.h"

class Graphics;
struct In;

struct SpecifyRequest
{
  enum SPECIFY_REQUEST_TYPE
  {
    NONE,
    CONFIRM_ACTION,
    CANCEL_SPECIFY,
    COUNT
  } type;
  Action *action;
};

class SpecifyTurnPicker
{
  private:
    Turn *turn;
    Action *action;

    UI::Label titleLabel;
    UI::Image power_0;
    UI::Image power_1;

    UI::Image action_image;
    UI::Image action_power_0;
    UI::Image action_power_1;
    UI::Label action_title;
    UI::Label action_description_0;
    UI::Label action_description_1;

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

