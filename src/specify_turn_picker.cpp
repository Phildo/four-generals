#include "specify_turn_picker.h"
#include "graphics.h"
#include "input.h"

SpecifyRequest AttackSpecifier::touch(In &in)
{
  SpecifyRequest s;
  return s;
}
void AttackSpecifier::tick()
{

}
void AttackSpecifier::draw(Graphics *g)
{

}

SpecifyRequest DefendSpecifier::touch(In &in)
{
  SpecifyRequest s;
  return s;
}
void DefendSpecifier::tick()
{

}
void DefendSpecifier::draw(Graphics *g)
{

}

SpecifyRequest MessageSpecifier::touch(In &in)
{
  SpecifyRequest s;
  return s;
}
void MessageSpecifier::tick()
{

}
void MessageSpecifier::draw(Graphics *g)
{

}

SpecifyRequest SabotageSpecifier::touch(In &in)
{
  SpecifyRequest s;
  return s;
}
void SabotageSpecifier::tick()
{

}
void SabotageSpecifier::draw(Graphics *g)
{

}

SpecifyRequest ScoutSpecifier::touch(In &in)
{
  SpecifyRequest s;
  return s;
}
void ScoutSpecifier::tick()
{

}
void ScoutSpecifier::draw(Graphics *g)
{

}


SpecifyTurnPicker::SpecifyTurnPicker()
{

}

SpecifyTurnPicker::SpecifyTurnPicker(Turn *t, UI::Box b)
{
  turn = t;

  box = b;
  titleLabel = UI::Label("Your Move:",box.x+10,box.y+10,34);
  power_0 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40-5-40, box.y+10, 40,40);
  power_1 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40     , box.y+10, 40,40);

  action_image   = UI::Image(Sprite::sword, box.x+10, box.y+50, 100,100);

  action_power_0 = UI::Image(Sprite::bolt, action_image.rect.rect);
  action_power_0.rect.x = action_power_0.rect.x+action_power_0.rect.w-60;
  action_power_0.rect.y = action_power_0.rect.y+action_power_0.rect.h-40;
  action_power_0.rect.w = 30;
  action_power_0.rect.h = 30;

  action_power_1 = UI::Image(Sprite::bolt, action_image.rect.rect);
  action_power_1.rect.x = action_power_1.rect.x+action_power_1.rect.w-40;
  action_power_1.rect.y = action_power_1.rect.y+action_power_1.rect.h-40;
  action_power_1.rect.w = 30;
  action_power_1.rect.h = 30;

  action_title         = UI::Label("Action",     action_image.rect.x+action_image.rect.w+10,action_image.rect.y,34);
  action_description_0 = UI::Label("So good tho",action_image.rect.x+action_image.rect.w+10,action_image.rect.y+40,25);
  action_description_1 = UI::Label("So good tho",action_image.rect.x+action_image.rect.w+10,action_image.rect.y+70,25);

  cancel  = UI::TextButton("Cancel", box.x+(box.w/2)-100-50,box.y+box.h-15,100,30);
  confirm = UI::TextButton("Confirm",box.x+(box.w/2)+100-50,box.y+box.h-15,100,30);
}

SpecifyTurnPicker::~SpecifyTurnPicker()
{
}

SpecifyRequest SpecifyTurnPicker::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;

  if(in.type == In::DOWN && cancel.query(in))
    s.type = SpecifyRequest::CANCEL_SPECIFY;

  switch(action->what)
  {
    case 'a':
      attack_specifier.touch(in);
      break;
    case 'd':
      defend_specifier.touch(in);
      break;
    case 'm':
      message_specifier.touch(in);
      break;
    case 's':
      sabotage_specifier.touch(in);
      break;
    case 'c':
      scout_specifier.touch(in);
      break;
    default:
      break;
  }

  return s;
}

void SpecifyTurnPicker::tick()
{
  switch(action->what)
  {
    case 'a':
      attack_specifier.tick();
      break;
    case 'd':
      defend_specifier.tick();
      break;
    case 'm':
      message_specifier.tick();
      break;
    case 's':
      sabotage_specifier.tick();
      break;
    case 'c':
      scout_specifier.tick();
      break;
    default:
      break;
  }
}

void SpecifyTurnPicker::draw(Graphics *g)
{
  box.draw(g);
  titleLabel.draw(g);
  power_0.draw(g);
  power_1.draw(g);

  action_image.draw(g);
  action_power_0.draw(g);
  action_power_1.draw(g);
  action_title.draw(g);
  action_description_0.draw(g);
  action_description_1.draw(g);

  switch(action->what)
  {
    case 'a':
      attack_specifier.draw(g);
      break;
    case 'd':
      defend_specifier.draw(g);
      break;
    case 'm':
      message_specifier.draw(g);
      break;
    case 's':
      sabotage_specifier.draw(g);
      break;
    case 'c':
      scout_specifier.draw(g);
      break;
    default:
      break;
  }

  cancel.draw(g);
  confirm.draw(g);
}

void SpecifyTurnPicker::setAction(Action *a)
{
  action = a;

  switch(action->what)
  {
    case 'a':
      action_image.sprite = Sprite::sword;
      action_power_1.sprite = Sprite::bolt;
      action_title.text = String("Attack");
      action_description_0.text = String("Attacking is good.");
      action_description_1.text = String("Like seriously hella good");
      attack_specifier.action = a;
      break;
    case 'd':
      action_image.sprite = Sprite::shield;
      action_power_1.sprite = Sprite::bolt;
      action_title.text = String("Fortify");
      action_description_0.text = String("You like defend and shit");
      action_description_1.text = String("Dude it's a good idea do it.");
      attack_specifier.action = a;
      break;
    case 'm':
      action_image.sprite = Sprite::envelope;
      action_power_1.sprite = Sprite::bolt_empty;
      action_title.text = String("Message");
      action_description_0.text = String("Send a message to your homies");
      action_description_1.text = String("Your homies haven't heard from you in forever");
      message_specifier.action = a;
      break;
    case 's':
      action_image.sprite = Sprite::knife;
      action_power_1.sprite = Sprite::bolt_empty;
      action_title.text = String("Sabotage");
      action_description_0.text = String("Stop that messenger");
      action_description_1.text = String("Stop him right there.");
      sabotage_specifier.action = a;
      break;
    case 'c':
      action_image.sprite = Sprite::knife;
      action_power_1.sprite = Sprite::bolt_empty;
      action_title.text = String("Scout");
      action_description_0.text = String("Dawg you can see everything");
      action_description_1.text = String("But first, you must look inside yourself");
      scout_specifier.action = a;
      break;
    default:
      break;
  }
}

void SpecifyTurnPicker::deactivate()
{

}

void SpecifyTurnPicker::activate()
{

}

