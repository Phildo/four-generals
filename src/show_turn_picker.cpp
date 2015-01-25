#include "show_turn_picker.h"
#include "graphics.h"
#include "input.h"

ShowTurnPicker::ShowTurnPicker()
{

}

ShowTurnPicker::ShowTurnPicker(UI::Box b)
{
  box = b;
  titleLabel = UI::Label("Your Move:",box.x+10,box.y+10,34);
  power_0 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40-5-40, box.y+10, 40,40);
  power_1 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40     , box.y+10, 40,40);
  add = UI::ImageButtonRound(Sprite::plus, box.x+(box.w/2)-50, box.y+50, 100, 100);

  cancel  = UI::TextButton("Cancel", box.x+(box.w/2)-100-50,box.y+box.h-15,100,30);
  confirm = UI::TextButton("Confirm",box.x+(box.w/2)+100-50,box.y+box.h-15,100,30);
}

ShowTurnPicker::~ShowTurnPicker()
{
}

void ShowTurnPicker::touch(In &in)
{
}

void ShowTurnPicker::tick()
{

}

void ShowTurnPicker::draw(Turn t, Graphics *g)
{
  box.draw(g);
  titleLabel.draw(g);
  power_0.draw(g);
  power_1.draw(g);
  add.draw(g);

  cancel.draw(g);
  confirm.draw(g);
}

