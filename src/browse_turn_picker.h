#ifndef _FG_BROWSE_TURN_PICKER_H_
#define _FG_BROWSE_TURN_PICKER_H_

#include "turn.h"
#include "ui.h"

class Graphics;
struct In;

struct BrowseRequest
{
  enum SHOW_REQUEST_TYPE
  {
    NONE,
    SPECIFY_ACTION,
    CANCEL_BROWSE,
    COUNT
  } type;
  int action; //0 or 1
};

struct BrowseButton
{
  UI::Button button;
  UI::Image image;
  UI::Image power_0;
  UI::Image power_1;
  UI::Label title;
  UI::Label description;

  void drawAtOffsetInMask(int o, Graphics *g, SDL_Rect r);
};

class BrowseTurnPicker
{
  private:
    Turn *turn;

    UI::Label titleLabel;
    UI::Image power_0;
    UI::Image power_1;
    UI::Scroller scroll;

    BrowseButton attack;
    BrowseButton defend;
    BrowseButton message;
    BrowseButton sabotage;
    BrowseButton scout;

    UI::TextButton cancel;

  public:
    UI::Box box;
    BrowseTurnPicker();
    BrowseTurnPicker(Turn *t, UI::Box wbox);
    ~BrowseTurnPicker();

    void touch(In &in);
    void tick();
    void draw(Graphics *g);
};

#endif

