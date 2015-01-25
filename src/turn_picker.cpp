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
  titleLabel = UI::Label("Your Move:",box.x+10,box.y+10,34);
  power_0 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40-5-40, box.y+10, 40,40);
  power_1 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40     , box.y+10, 40,40);
  add = UI::ImageButtonRound(Sprite::plus, box.x+(box.w/2)-50, box.y+50, 100, 100);

  cancel  = UI::TextButton("Cancel", box.x+(box.w/2)-100-50,box.y+box.h-15,100,30);
  confirm = UI::TextButton("Confirm",box.x+(box.w/2)+100-50,box.y+box.h-15,100,30);
}

TurnPicker::~TurnPicker()
{
}

void TurnPicker::touch(In &in)
{

}

bool TurnPicker::query(In &in)
{
  return false;
}

void TurnPicker::tick()
{

}

void TurnPicker::draw(Graphics *g)
{
  box.draw(g);
  titleLabel.draw(g);
  power_0.draw(g);
  power_1.draw(g);
  add.draw(g);
  cancel.draw(g);
  confirm.draw(g);
}

void TurnPicker::setViewState(TURN_PICKER_STATE s)
{

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

