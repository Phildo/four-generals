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
  char cardinal; \
  UI::Box rect; \
  UI::TextButton confirm; \
  void init(SDL_Rect r); \
  void setCardinal(char c); \
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

#define SPECIFIER_WHO_E_SPEC \
  UI::Label who; \
  UI::ImageButton who_cw; \
  UI::ImageButton who_ccw;

#define SPECIFIER_WHO_F_SPEC \
  UI::Label who; \
  UI::ImageButton who_me; \
  UI::ImageButton who_p;

#define SPECIFIER_WHEN_SPEC \
  UI::Label when; \
  UI::ImageButton when_su; UI::Label when_su_l; \
  UI::ImageButton when_mo; UI::Label when_mo_l; \
  UI::ImageButton when_tu; UI::Label when_tu_l; \
  UI::ImageButton when_we; UI::Label when_we_l; \
  UI::ImageButton when_th; UI::Label when_th_l; \
  UI::ImageButton when_fr; UI::Label when_fr_l; \
  UI::ImageButton when_sa; UI::Label when_sa_l;

#define SPECIFIER_ROUTE_SPEC \
  UI::Label route; \
  UI::ImageButton route_cw; \
  UI::ImageButton route_ccw;

struct AttackSpecifier
{
  SPECIFIER_FUNC_SPEC
  SPECIFIER_WHO_E_SPEC
  UI::Label confirm_label;
  UI::ImageButton confirm_attack;
  UI::ImageButton confirm_who;
};
struct DefendSpecifier
{
  SPECIFIER_FUNC_SPEC
  UI::Label confirm_label;
  UI::ImageButton confirm_defend;
};
struct MessageSpecifier
{
  SPECIFIER_FUNC_SPEC
  SPECIFIER_WHO_E_SPEC
  SPECIFIER_WHEN_SPEC
  SPECIFIER_ROUTE_SPEC
  UI::Label confirm_label;
  UI::ImageButton confirm_message;
  UI::ImageButton confirm_who;
  UI::ImageButton confirm_when;
  UI::Label confirm_when_l;
  UI::ImageButton confirm_route;
};
struct SabotageSpecifier
{
  SPECIFIER_FUNC_SPEC
  SPECIFIER_HOW_SPEC
  SPECIFIER_WHICH_SPEC
  SPECIFIER_WHO_F_SPEC
  SPECIFIER_WHEN_SPEC
  UI::Label confirm_label;
  UI::ImageButton confirm_sabotage;
  UI::ImageButton confirm_how;
  UI::ImageButton confirm_which;
  UI::ImageButton confirm_who;
  UI::ImageButton confirm_when;
  UI::Label confirm_when_l;
};
struct ScoutSpecifier
{
  SPECIFIER_FUNC_SPEC
  UI::Label confirm_label;
  UI::ImageButton confirm_scout;
};

class SpecifyTurnPicker
{
  private:
    Turn *turn;
    Action *action;
    char cardinal;

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

    void setCardinal(char c);
    SpecifyRequest touch(In &in);
    void tick();
    void draw(Graphics *g);

    void setAction(Action *a);
    void setPower(int p);
    void deactivate();
    void activate();
};

#endif

