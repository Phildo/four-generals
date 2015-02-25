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
  action_0_1 = UI::ImageButtonRound(Sprite::plus, box.x+(box.w/2)-50,           box.y+50, 100, 100);
  action_0_2 = UI::ImageButtonRound(Sprite::plus, box.x+space(box.w,0,100,2,0), box.y+50, 100, 100);
  action_1_2 = UI::ImageButtonRound(Sprite::plus, box.x+space(box.w,0,100,2,1), box.y+50, 100, 100);

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
  if(turn->actions[0].power() == 0 || turn->actions[0].power() == 2)
  {
    if(action_0_1.query(in))
    {
      s.action = &turn->actions[0];
      if(s.action->power() == 0)
        s.type = ShowRequest::ADD_ACTION;
      else
        s.type = ShowRequest::EDIT_ACTION;
    }
  }
  else //turn->actions[1].power() == 1
  {
    if(action_0_2.query(in))
    {
      s.action = &turn->actions[0];
      s.type = ShowRequest::EDIT_ACTION; //know its power > 0
    }
    if(action_1_2.query(in))
    {
      s.action = &turn->actions[1];
      if(s.action->power() == 0)
        s.type = ShowRequest::ADD_ACTION;
      else
        s.type = ShowRequest::EDIT_ACTION;
    }
  }

  if(cancel.query(in))  s.type = ShowRequest::CANCEL;
  if(confirm.query(in)) s.type = ShowRequest::CONFIRM;

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

void ShowTurnPicker::setPower(int p)
{
  if(p > 0) power_0.sprite = Sprite::bolt; else power_0.sprite = Sprite::bolt_empty;
  if(p > 1) power_1.sprite = Sprite::bolt; else power_1.sprite = Sprite::bolt_empty;
}

void ShowTurnPicker::deactivate()
{

}

void ShowTurnPicker::activate()
{
  switch(turn->actions[0].what)
  {
    case '0':
      action_0_1.sprite = Sprite::plus;
      action_0_2.sprite = Sprite::plus;
      break;
    case 'a':
      action_0_1.sprite = Sprite::sword;
      action_0_2.sprite = Sprite::sword;
      break;
    case 'd':
      action_0_1.sprite = Sprite::shield;
      action_0_2.sprite = Sprite::shield;
      break;
    case 'm':
      action_0_1.sprite = Sprite::envelope;
      action_0_2.sprite = Sprite::envelope;
      break;
    case 's':
      action_0_1.sprite = Sprite::knife;
      action_0_2.sprite = Sprite::knife;
      break;
    case 'c':
      action_0_1.sprite = Sprite::telescope;
      action_0_2.sprite = Sprite::telescope;
      break;
  }

  switch(turn->actions[1].what)
  {
    case '0':
      action_1_2.sprite = Sprite::plus;
      break;
    case 'm':
      action_1_2.sprite = Sprite::envelope;
      break;
    case 's':
      action_1_2.sprite = Sprite::knife;
      break;
    case 'c':
      action_1_2.sprite = Sprite::telescope;
      break;
  }
}

