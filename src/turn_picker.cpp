#include "turn_picker.h"
#include "graphics.h"
#include "input.h"

TurnPicker::TurnPicker() : win_box(0,0,100,100)
{
}

TurnPicker::TurnPicker(UI::Box wBox) : win_box(wBox)
{
}

void TurnPicker::init()
{
  int w;
  int h;
  UI::Box b;

  w = 400;
  h = 180;
  b = UI::Box(win_box.x+(win_box.w/2)-(w/2),win_box.y+(win_box.h/2)-(h/2),w,h);
  show = ShowTurnPicker(&turn, b.rect);
  w = win_box.w-100;
  h = win_box.h-100;
  b = UI::Box(win_box.x+(win_box.w/2)-(w/2),win_box.y+(win_box.h/2)-(h/2),w,h);
  browse = BrowseTurnPicker(&turn, b.rect);
  w = win_box.w-40;
  h = win_box.h-40;
  b = UI::Box(win_box.x+(win_box.w/2)-(w/2),win_box.y+(win_box.h/2)-(h/2),w,h);
  specify = SpecifyTurnPicker(b.rect);

  setViewState(SHOW);
}

TurnPicker::~TurnPicker()
{
}

void TurnPicker::touch(In &in)
{
  ShowRequest s;
  switch(state)
  {
    case SHOW:
      s = show.touch(in);
      if(s.type == ShowRequest::ADD_ACTION) setViewState(BROWSE);
    break;
    case BROWSE:
      browse.touch(in);
    break;
    case SPECIFY:
      specify.touch(in);
    break;
    default:
    break;
  }
}

bool TurnPicker::query(In &in)
{
  return button.query(in);
}

void TurnPicker::tick()
{
  switch(state)
  {
    case SHOW:
      show.tick();
      break;
    case BROWSE:
      browse.tick();
      break;
    case SPECIFY:
      specify.tick();
      break;
    default:
      break;
  }

}

void TurnPicker::draw(Graphics *g)
{
  switch(state)
  {
    case SHOW:
      show.draw(g);
      break;
    case BROWSE:
      browse.draw(g);
      break;
    case SPECIFY:
      specify.draw(turn,g);
      break;
    default:
      break;
  }
}

void TurnPicker::setViewState(TURN_PICKER_STATE s)
{
  state = s;
  switch(state)
  {
    case SHOW:
      box.rect = show.box.rect;
      button.rect = show.box.rect;
      break;
    case BROWSE:
      box.rect = browse.box.rect;
      button.rect = browse.box.rect;
      break;
    case SPECIFY:
      box.rect = specify.box.rect;
      button.rect = specify.box.rect;
      break;
    default:
      break;
  }

  //set box/button
}

void TurnPicker::clearViewState()
{

}

bool TurnPicker::getTurn(Turn &t)
{
  return false;
}

void TurnPicker::reset()
{

}

