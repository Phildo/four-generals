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
  w = win_box.w-200;
  h = win_box.h-200;
  b = UI::Box(win_box.x+(win_box.w/2)-(w/2),win_box.y+(win_box.h/2)-(h/2),w,h);
  browse = BrowseTurnPicker(&turn, b.rect);
  w = win_box.w-200;
  h = win_box.h-200;
  b = UI::Box(win_box.x+(win_box.w/2)-(w/2),win_box.y+(win_box.h/2)-(h/2),w,h);
  specify = SpecifyTurnPicker(&turn, b.rect);

  setViewState(SHOW);
}

void TurnPicker::setCardinal(char c)
{
  cardinal = c;
  specify.setCardinal(c);
}

TurnPicker::~TurnPicker()
{
}

void TurnPicker::touch(In &in)
{
  ShowRequest s;
  BrowseRequest b;
  SpecifyRequest sp;
  switch(state)
  {
    case SHOW:
      s = show.touch(in);
      if(s.type == ShowRequest::ADD_ACTION)
      {
        if(turn.actions[0].power() == 0) browse.setAction(&turn.actions[0]);
        else                             browse.setAction(&turn.actions[1]);
        setViewState(BROWSE);
      }
    break;
    case BROWSE:
      b = browse.touch(in);
      if(b.type == BrowseRequest::SPECIFY_ACTION)
      {
        specify.setAction(b.action);
        setViewState(SPECIFY);
      }
      if(b.type == BrowseRequest::CANCEL_BROWSE)
      {
        b.action->zero();
        setViewState(SHOW);
      }
    break;
    case SPECIFY:
      sp = specify.touch(in);
      if(sp.type == SpecifyRequest::CANCEL_SPECIFY)
      {
        sp.action->zero();
        setViewState(BROWSE);
      }
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
      specify.draw(g);
      break;
    default:
      break;
  }
}

void TurnPicker::setViewState(TURN_PICKER_STATE s)
{
  switch(state)
  {
    case SHOW:
      show.deactivate();
      break;
    case BROWSE:
      browse.deactivate();
      break;
    case SPECIFY:
      specify.deactivate();
      break;
    default:
      break;
  }
  state = s;
  switch(state)
  {
    case SHOW:
      box.rect = show.box.rect;
      button.rect = show.box.rect;
      show.activate();
      break;
    case BROWSE:
      box.rect = browse.box.rect;
      button.rect = browse.box.rect;
      browse.activate();
      break;
    case SPECIFY:
      box.rect = specify.box.rect;
      button.rect = specify.box.rect;
      specify.activate();
      break;
    default:
      break;
  }
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

