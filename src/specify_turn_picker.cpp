#include "specify_turn_picker.h"
#include "graphics.h"
#include "input.h"

#define SPECIFIER_HOW_IMPL \
  how = UI::Label("How will you sabotage?",r.x+10,r.y+10,35); \
  how_block  = UI::ImageButton(Sprite::sblock,  r.x+space(r.w,0,100,3,0), r.y+50, 100, 100); \
  how_read   = UI::ImageButton(Sprite::sread,   r.x+space(r.w,0,100,3,1), r.y+50, 100, 100); \
  how_switch = UI::ImageButton(Sprite::sswitch, r.x+space(r.w,0,100,3,2), r.y+50, 100, 100);

#define SPECIFIER_WHICH_IMPL \
  which = UI::Label("Which data will you switch?",r.x+10,r.y+10,35); \
  which_who  = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  which_when = UI::ImageButton(Sprite::sun,    r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);

#define SPECIFIER_WHO_IMPL \
  who = UI::Label("Who?",r.x+10,r.y+10,35); \
  who_cw  = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  who_ccw = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);

#define SPECIFIER_WHEN_IMPL \
  when = UI::Label("When?",r.x+10,r.y+10,35); \
  when_su = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,0), r.y+50, 100, 100); \
  when_mo = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,1), r.y+50, 100, 100); \
  when_tu = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,2), r.y+50, 100, 100); \
  when_we = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,3), r.y+50, 100, 100); \
  when_th = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,4), r.y+50, 100, 100); \
  when_fr = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,5), r.y+50, 100, 100); \
  when_sa = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,6), r.y+50, 100, 100);

#define SPECIFIER_ROUTE_IMPL \
  route = UI::Label("Which route should the messenger take?",r.x+10,r.y+10,35); \
  route_cw  = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  route_ccw = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);



#define SPECIFIER_DRAW_HOW_IMPL \
{ \
  how.draw(g); \
  how_block.draw(g); \
  how_read.draw(g); \
  how_switch.draw(g); \
}

#define SPECIFIER_DRAW_WHICH_IMPL \
{ \
  which.draw(g); \
  which_who.draw(g); \
  which_when.draw(g); \
}

#define SPECIFIER_DRAW_WHO_IMPL \
{ \
  who.draw(g); \
  who_cw.draw(g); \
  who_ccw.draw(g); \
}

#define SPECIFIER_DRAW_WHEN_IMPL \
{ \
  when.draw(g); \
  when_su.draw(g); \
  when_mo.draw(g); \
  when_tu.draw(g); \
  when_we.draw(g); \
  when_th.draw(g); \
  when_fr.draw(g); \
  when_sa.draw(g); \
}

#define SPECIFIER_DRAW_ROUTE_IMPL \
{ \
  route.draw(g); \
  route_cw.draw(g); \
  route_ccw.draw(g); \
}



void AttackSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  SPECIFIER_WHO_IMPL
}
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
  rect.draw(g);
  if(action->who == '0')
    SPECIFIER_DRAW_WHO_IMPL
}

void DefendSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);

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

void MessageSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  SPECIFIER_WHO_IMPL
  SPECIFIER_WHEN_IMPL
  SPECIFIER_ROUTE_IMPL
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
  if(action->who == '0')
    SPECIFIER_DRAW_WHO_IMPL
  else if(action->when == '0')
    SPECIFIER_DRAW_WHEN_IMPL
  else if(action->route == '0')
    SPECIFIER_DRAW_ROUTE_IMPL
}

void SabotageSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  SPECIFIER_HOW_IMPL
  SPECIFIER_WHICH_IMPL
  SPECIFIER_WHO_IMPL
  SPECIFIER_WHEN_IMPL
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
  if(action->how == '0')
  {
    how.draw(g);
    how_block.draw(g);
    how_read.draw(g);
    how_switch.draw(g);
  }
  else if(action->how == 's')
  {
    if(action->which == '0')
      SPECIFIER_DRAW_WHICH_IMPL
    else if(action->which == 'o')
      SPECIFIER_DRAW_WHO_IMPL
    else if(action->which == 'e')
      SPECIFIER_DRAW_WHEN_IMPL
  }
}

void ScoutSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
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

  SDL_Rect r;
  r.x = b.x+10;
  r.w = b.w-20;
  r.h = b.h/2;
  r.y = b.y+b.h-r.h-10;

  attack_specifier.init(r);
  defend_specifier.init(r);
  message_specifier.init(r);
  sabotage_specifier.init(r);
  scout_specifier.init(r);
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

