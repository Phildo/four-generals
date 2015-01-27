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

#define SPECIFIER_FUNC_SPEC \
  Action *action; \
  UI::Box rect; \
  void init(SDL_Rect r); \
  SpecifyRequest touch(In &in); \
  void tick(); \
  void draw(Graphics *g);

#define SPECIFIER_HOW_SPEC \
  UI::Label how; \
  UI::ImageButton how_block; \
  UI::ImageButton how_read; \
  UI::ImageButton how_switch;

#define SPECIFIER_WHICH_SPEC \
  UI::Label which; \
  UI::ImageButton which_who; \
  UI::ImageButton which_when;

#define SPECIFIER_WHO_SPEC \
  UI::Label who; \
  UI::ImageButton who_cw; \
  UI::ImageButton who_ccw;

#define SPECIFIER_WHEN_SPEC \
  UI::Label when; \
  UI::ImageButton when_su; \
  UI::ImageButton when_mo; \
  UI::ImageButton when_tu; \
  UI::ImageButton when_we; \
  UI::ImageButton when_th; \
  UI::ImageButton when_fr; \
  UI::ImageButton when_sa;

#define SPECIFIER_ROUTE_SPEC \
  UI::Label route; \
  UI::ImageButton route_cw; \
  UI::ImageButton route_ccw;

struct AttackSpecifier
{
  SPECIFIER_FUNC_SPEC
  SPECIFIER_WHO_SPEC
};
struct DefendSpecifier
{
  SPECIFIER_FUNC_SPEC
};
struct MessageSpecifier
{
  SPECIFIER_FUNC_SPEC
  SPECIFIER_WHO_SPEC
  SPECIFIER_WHEN_SPEC
  SPECIFIER_ROUTE_SPEC
};
struct SabotageSpecifier
{
  SPECIFIER_FUNC_SPEC
  SPECIFIER_HOW_SPEC
  SPECIFIER_WHICH_SPEC
  SPECIFIER_WHO_SPEC
  SPECIFIER_WHEN_SPEC
};
struct ScoutSpecifier
{
  SPECIFIER_FUNC_SPEC
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

    AttackSpecifier attack_specifier;
    DefendSpecifier defend_specifier;
    MessageSpecifier message_specifier;
    SabotageSpecifier sabotage_specifier;
    ScoutSpecifier scout_specifier;

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

