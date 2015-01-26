#include "show_turn_picker.h"
#include "graphics.h"
#include "input.h"

ShowTurnPicker::ShowTurnPicker()
{

}

ShowTurnPicker::ShowTurnPicker(Turn *t, UI::Box b)
{
  turn = t;

  box = b;
  titleLabel = UI::Label("Your Move:",box.x+10,box.y+10,34);
  power_0 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40-5-40, box.y+10, 40,40);
  power_1 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40     , box.y+10, 40,40);
  action_0_1 = UI::ImageButtonRound(Sprite::plus, box.x+(box.w/2)-50, box.y+50, 100, 100);
  action_0_2 = UI::ImageButtonRound(Sprite::plus, box.x+(box.w/2)-50, box.y+50, 100, 100);
  action_1_2 = UI::ImageButtonRound(Sprite::plus, box.x+(box.w/2)-50, box.y+50, 100, 100);

  cancel  = UI::TextButton("Cancel", box.x+(box.w/2)-100-50,box.y+box.h-15,100,30);
  confirm = UI::TextButton("Confirm",box.x+(box.w/2)+100-50,box.y+box.h-15,100,30);
}

ShowTurnPicker::~ShowTurnPicker()
{
}

ShowRequest ShowTurnPicker::touch(In &in)
{
  ShowRequest s;
  s.type = ShowRequest::NONE;
  if(turn->actions[0].power() == 0)
  {
    if(action_0_1.query(in))
    {
      s.type = ShowRequest::ADD_ACTION;
    }
  }
  return s;
}

void ShowTurnPicker::tick()
{

}

void ShowTurnPicker::draw(Graphics *g)
{
  box.draw(g);
  titleLabel.draw(g);
  power_0.draw(g);
  power_1.draw(g);
  if(turn->actions[0].power() == 0 || turn->actions[0].power() == 2)
    action_0_1.draw(g);
  else
  {
    action_0_2.draw(g);
    action_1_2.draw(g);
  }

  cancel.draw(g);
  confirm.draw(g);
}

void ShowTurnPicker::deactivate()
{

}

void ShowTurnPicker::activate()
{

}

