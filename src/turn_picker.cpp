#include "turn_picker.h"
#include "graphics.h"
#include "input.h"

TurnPicker::TurnPicker() : win_box(0,0,100,100)
{
  init();
}

TurnPicker::TurnPicker(UI::Box wBox) : win_box(wBox)
{
  init();
}

void TurnPicker::init()
{
  int w = 400;
  int h = 180;
  box = UI::Box(win_box.x+(win_box.w/2)-(w/2),win_box.y+(win_box.h/2)-(h/2),w,h);
  button = UI::Button(box.rect);
  show = ShowTurnPicker(&turn, box.rect);
  browse = BrowseTurnPicker(box.rect);
  specify = SpecifyTurnPicker(box.rect);

  setViewState(SHOW);
}

TurnPicker::~TurnPicker()
{
}

void TurnPicker::touch(In &in)
{
  switch(state)
  {
    case SHOW:
      show.touch(in);
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

}

void TurnPicker::draw(Graphics *g)
{
  switch(state)
  {
    case SHOW:
      show.draw(g);
      break;
    case BROWSE:
      browse.draw(turn,g);
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

