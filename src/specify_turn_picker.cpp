#include "specify_turn_picker.h"
#include "graphics.h"
#include "input.h"
#include "compass.h"

#define SPECIFIER_WHICH_IMPL \
  which = UI::Label("Which data will you switch?",r.x+10,r.y+10,35); \
  which_who  = UI::ImageButton(Sprite::sword, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  which_when = UI::ImageButton(Sprite::sun,   r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);

#define SPECIFIER_WHO_E_IMPL \
  who = UI::Label("Who?",r.x+10,r.y+10,35); \
  who_cw  = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  who_ccw = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);

#define SPECIFIER_WHO_F_IMPL \
  who = UI::Label("Who?",r.x+10,r.y+10,35); \
  who_me = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  who_p  = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);

#define SPECIFIER_WHEN_IMPL \
  when = UI::Label("When?",r.x+10,r.y+10,35); \
  when_su = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,0), r.y+50, 100, 100); when_su_l = UI::Label("Sun", r.x+space(r.w,0,100,7,0), r.y+50, 50); \
  when_mo = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,1), r.y+50, 100, 100); when_mo_l = UI::Label("Mon", r.x+space(r.w,0,100,7,1), r.y+50, 50); \
  when_tu = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,2), r.y+50, 100, 100); when_tu_l = UI::Label("Tue", r.x+space(r.w,0,100,7,2), r.y+50, 50); \
  when_we = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,3), r.y+50, 100, 100); when_we_l = UI::Label("Wed", r.x+space(r.w,0,100,7,3), r.y+50, 50); \
  when_th = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,4), r.y+50, 100, 100); when_th_l = UI::Label("Thu", r.x+space(r.w,0,100,7,4), r.y+50, 50); \
  when_fr = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,5), r.y+50, 100, 100); when_fr_l = UI::Label("Fri", r.x+space(r.w,0,100,7,5), r.y+50, 50); \
  when_sa = UI::ImageButton(Sprite::sun, r.x+space(r.w,0,100,7,6), r.y+50, 100, 100); when_sa_l = UI::Label("Sat", r.x+space(r.w,0,100,7,6), r.y+50, 50);

#define SPECIFIER_ROUTE_IMPL \
  route = UI::Label("Which route should the messenger take?",r.x+10,r.y+10,35); \
  route_cw  = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100); \
  route_ccw = UI::ImageButton(Sprite::sblock, r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);



#define SPECIFIER_DRAW_WHICH_IMPL \
{ \
  which.draw(g); \
  which_who.draw(g); \
  which_when.draw(g); \
}

#define SPECIFIER_DRAW_WHO_E_IMPL \
{ \
  who.draw(g); \
  who_cw.draw(g); \
  who_ccw.draw(g); \
}

#define SPECIFIER_DRAW_WHO_F_IMPL \
{ \
  who.draw(g); \
  who_me.draw(g); \
  who_p.draw(g); \
}

#define SPECIFIER_DRAW_WHEN_IMPL \
{ \
  when.draw(g); \
  when_su.draw(g); when_su_l.draw(g); \
  when_mo.draw(g); when_mo_l.draw(g); \
  when_tu.draw(g); when_tu_l.draw(g); \
  when_we.draw(g); when_we_l.draw(g); \
  when_th.draw(g); when_th_l.draw(g); \
  when_fr.draw(g); when_fr_l.draw(g); \
  when_sa.draw(g); when_sa_l.draw(g); \
}

#define SPECIFIER_DRAW_ROUTE_IMPL \
{ \
  route.draw(g); \
  route_cw.draw(g); \
  route_ccw.draw(g); \
}


#define SPECIFIER_TOUCH_WHICH_IMPL \
{ \
  if(which_who.query(in))  { action->which = 'o'; confirm_which.sprite = Sprite::sword; }  \
  if(which_when.query(in)) { action->which = 'e'; confirm_which.sprite = Sprite::sun; } \
}

#define SPECIFIER_TOUCH_WHO_E_IMPL \
{ \
  if(who_cw.query(in))  action->who = Compass::cwcardinal(cardinal); \
  if(who_ccw.query(in)) action->who = Compass::ccwcardinal(cardinal); \
  confirm_who.sprite = Sprite::generals[Compass::icardinal(action->who)]; \
}

#define SPECIFIER_TOUCH_WHO_F_IMPL \
{ \
  if(who_me.query(in))  action->who = cardinal; \
  if(who_p.query(in)) action->who = Compass::opcardinal(cardinal); \
  confirm_who.sprite = Sprite::generals[Compass::icardinal(action->who)]; \
}

#define SPECIFIER_TOUCH_WHEN_IMPL \
{ \
  if(when_su.query(in)) { action->when = 's'; confirm_when_l.text = String("Sun"); } \
  if(when_mo.query(in)) { action->when = 'm'; confirm_when_l.text = String("Mon"); } \
  if(when_tu.query(in)) { action->when = 't'; confirm_when_l.text = String("Tue"); } \
  if(when_we.query(in)) { action->when = 'w'; confirm_when_l.text = String("Wed"); } \
  if(when_th.query(in)) { action->when = 'h'; confirm_when_l.text = String("Thu"); } \
  if(when_fr.query(in)) { action->when = 'f'; confirm_when_l.text = String("Fri"); } \
  if(when_sa.query(in)) { action->when = 'a'; confirm_when_l.text = String("Sat"); } \
}

#define SPECIFIER_TOUCH_ROUTE_IMPL \
{ \
  if(route_cw.query(in))  action->route = Compass::cwcardinal(cardinal); \
  if(route_ccw.query(in)) action->route = Compass::ccwcardinal(cardinal); \
  confirm_route.sprite = Sprite::generals[Compass::icardinal(action->route)]; \
}



void AttackSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  confirm = UI::TextButton("Confirm",r.x+(r.w/2)+100-50,r.y+r.h-15,100,30);
  SPECIFIER_WHO_E_IMPL
  confirm_label = UI::Label("Confirm?",r.x+10,r.y+10,35);
  confirm_attack = UI::ImageButton(Sprite::sword, r.x+space(r.w,0,100,2,0), r.y+50, 100, 100);
  confirm_who    = UI::ImageButton(Sprite::sread, r.x+space(r.w,0,100,2,1), r.y+50, 100, 100);
}
void AttackSpecifier::setCardinal(char c)
{
  cardinal = c;
  who_cw.sprite = Sprite::generals[Compass::icardinal(Compass::cwcardinal(cardinal))];
  who_ccw.sprite = Sprite::generals[Compass::icardinal(Compass::ccwcardinal(cardinal))];
}
SpecifyRequest AttackSpecifier::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;

  if(action->who == '0')
  {
    SPECIFIER_TOUCH_WHO_E_IMPL
  }
  else
  {
    if(confirm_attack.query(in)) s.type = SpecifyRequest::CANCEL_SPECIFY;
    if(confirm_who.query(in)) action->who = '0';
    if(confirm.query(in)) s.type = SpecifyRequest::CONFIRM_ACTION;
  }

  return s;
}
void AttackSpecifier::tick()
{

}
void AttackSpecifier::draw(Graphics *g)
{
  if(action->who == '0')
    SPECIFIER_DRAW_WHO_E_IMPL
  else
  {
    confirm_label.draw(g);
    confirm_attack.draw(g);
    confirm_who.draw(g);
    confirm.draw(g);
  }
}

void DefendSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  confirm = UI::TextButton("Confirm",r.x+(r.w/2)+100-50,r.y+r.h-15,100,30);
  confirm_label = UI::Label("Confirm?",r.x+10,r.y+10,35);
  confirm_defend = UI::ImageButton(Sprite::shield, r.x+space(r.w,0,100,1,0), r.y+50, 100, 100);
}
void DefendSpecifier::setCardinal(char c)
{
  cardinal = c;
}
SpecifyRequest DefendSpecifier::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;

  if(confirm_defend.query(in)) s.type = SpecifyRequest::CANCEL_SPECIFY;
  if(confirm.query(in)) s.type = SpecifyRequest::CONFIRM_ACTION;

  return s;
}
void DefendSpecifier::tick()
{

}
void DefendSpecifier::draw(Graphics *g)
{
  confirm_label.draw(g);
  confirm_defend.draw(g);
  confirm.draw(g);
}

void MessageSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  confirm = UI::TextButton("Confirm",r.x+(r.w/2)+100-50,r.y+r.h-15,100,30);
  SPECIFIER_WHO_E_IMPL
  SPECIFIER_WHEN_IMPL
  SPECIFIER_ROUTE_IMPL
  confirm_label = UI::Label("Confirm?",r.x+10,r.y+10,35);
  confirm_message = UI::ImageButton(Sprite::envelope, r.x+space(r.w,0,100,4,0), r.y+50, 100, 100);
  confirm_who     = UI::ImageButton(Sprite::shield,   r.x+space(r.w,0,100,4,1), r.y+50, 100, 100);
  confirm_when    = UI::ImageButton(Sprite::sun,      r.x+space(r.w,0,100,4,2), r.y+50, 100, 100);
  confirm_when_l  = UI::Label("Sun",                  r.x+space(r.w,0,100,4,2), r.y+50,  50);
  confirm_route   = UI::ImageButton(Sprite::shield,   r.x+space(r.w,0,100,4,3), r.y+50, 100, 100);
}
void MessageSpecifier::setCardinal(char c)
{
  cardinal = c;
  who_cw.sprite = Sprite::generals[Compass::icardinal(Compass::cwcardinal(cardinal))];
  who_ccw.sprite = Sprite::generals[Compass::icardinal(Compass::ccwcardinal(cardinal))];
  route_cw.sprite = Sprite::generals[Compass::icardinal(Compass::cwcardinal(cardinal))];
  route_ccw.sprite = Sprite::generals[Compass::icardinal(Compass::ccwcardinal(cardinal))];
}
SpecifyRequest MessageSpecifier::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;

  if(action->who == '0')
    SPECIFIER_TOUCH_WHO_E_IMPL
  else if(action->when == '0')
    SPECIFIER_TOUCH_WHEN_IMPL
  else if(action->route == '0')
    SPECIFIER_TOUCH_ROUTE_IMPL
  else
  {
    if(confirm_message.query(in)) s.type = SpecifyRequest::CANCEL_SPECIFY;
    if(confirm_who.query(in)) action->who = '0';
    if(confirm_when.query(in)) action->when = '0';
    if(confirm_route.query(in)) action->route = '0';
    if(confirm.query(in)) s.type = SpecifyRequest::CONFIRM_ACTION;
  }

  return s;
}
void MessageSpecifier::tick()
{

}
void MessageSpecifier::draw(Graphics *g)
{
  if(action->who == '0')
    SPECIFIER_DRAW_WHO_E_IMPL
  else if(action->when == '0')
    SPECIFIER_DRAW_WHEN_IMPL
  else if(action->route == '0')
    SPECIFIER_DRAW_ROUTE_IMPL
  else
  {
    confirm_label.draw(g);
    confirm_message.draw(g);
    confirm_who.draw(g);
    confirm_when.draw(g);
    confirm_when_l.draw(g);
    confirm_route.draw(g);
    confirm.draw(g);
  }
}

void SabotageSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  confirm = UI::TextButton("Confirm",r.x+(r.w/2)+100-50,r.y+r.h-15,100,30);
  SPECIFIER_WHICH_IMPL
  SPECIFIER_WHO_F_IMPL
  SPECIFIER_WHEN_IMPL
  confirm_label = UI::Label("Confirm?",r.x+10,r.y+10,35);
  confirm_sabotage = UI::ImageButton(Sprite::knife,  r.x+space(r.w,0,100,3,0), r.y+50, 100, 100);
  confirm_which    = UI::ImageButton(Sprite::sun,    r.x+space(r.w,0,100,3,1), r.y+50, 100, 100);
  confirm_who      = UI::ImageButton(Sprite::shield, r.x+space(r.w,0,100,3,2), r.y+50, 100, 100);
  confirm_when     = UI::ImageButton(Sprite::sun,    r.x+space(r.w,0,100,3,2), r.y+50, 100, 100);
  confirm_when_l   = UI::Label("Sun",                r.x+space(r.w,0,100,3,2), r.y+50,  50);
}
void SabotageSpecifier::setCardinal(char c)
{
  cardinal = c;
  who_me.sprite = Sprite::generals[Compass::icardinal(cardinal)];
  who_p.sprite = Sprite::generals[Compass::icardinal(Compass::opcardinal(cardinal))];
}
SpecifyRequest SabotageSpecifier::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;

  if(action->which == '0')
    SPECIFIER_TOUCH_WHICH_IMPL
  else if(action->which == 'o')
  {
    if(action->who == '0')
      SPECIFIER_TOUCH_WHO_F_IMPL
    else
    {
      if(confirm_sabotage.query(in)) s.type = SpecifyRequest::CANCEL_SPECIFY;
      if(confirm_which.query(in)) action->which = '0';
      if(confirm_who.query(in)) action->who = '0';
      if(confirm.query(in)) s.type = SpecifyRequest::CONFIRM_ACTION;
    }
  }
  else if(action->which == 'e')
  {
    if(action->when == '0')
       SPECIFIER_TOUCH_WHEN_IMPL
    else
    {
      if(confirm_sabotage.query(in)) s.type = SpecifyRequest::CANCEL_SPECIFY;
      if(confirm_which.query(in)) action->which = '0';
      if(confirm_when.query(in)) action->when = '0';
      if(confirm.query(in)) s.type = SpecifyRequest::CONFIRM_ACTION;
    }
  }

  return s;
}
void SabotageSpecifier::tick()
{

}
void SabotageSpecifier::draw(Graphics *g)
{
  if(action->which == '0')
    SPECIFIER_DRAW_WHICH_IMPL
  else if(action->which == 'o')
  {
    if(action->who == '0')
      SPECIFIER_DRAW_WHO_F_IMPL
    else
    {
      confirm_label.draw(g);
      confirm_sabotage.draw(g);
      confirm_which.draw(g);
      confirm_who.draw(g);
      confirm.draw(g);
    }
  }
  else if(action->which == 'e')
  {
    if(action->when == '0')
       SPECIFIER_DRAW_WHEN_IMPL
    else
    {
      confirm_label.draw(g);
      confirm_sabotage.draw(g);
      confirm_which.draw(g);
      confirm_when.draw(g);
      confirm_when_l.draw(g);
      confirm.draw(g);
    }
  }
}

void ScoutSpecifier::init(SDL_Rect r)
{
  rect = UI::Box(r);
  confirm = UI::TextButton("Confirm",r.x+(r.w/2)+100-50,r.y+r.h-15,100,30);
  confirm_label = UI::Label("Confirm?",r.x+10,r.y+10,35);
  confirm_scout = UI::ImageButton(Sprite::telescope, r.x+space(r.w,0,100,1,0), r.y+50, 100, 100);
}
void ScoutSpecifier::setCardinal(char c)
{
  cardinal = c;
}
SpecifyRequest ScoutSpecifier::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;

  if(confirm_scout.query(in)) s.type = SpecifyRequest::CANCEL_SPECIFY;
  if(confirm.query(in)) s.type = SpecifyRequest::CONFIRM_ACTION;

  return s;
}
void ScoutSpecifier::tick()
{

}
void ScoutSpecifier::draw(Graphics *g)
{
  confirm_label.draw(g);
  confirm_scout.draw(g);
  confirm.draw(g);
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

  cancel  = UI::TextButton("Cancel",  box.x+(box.w/2)-100-50,box.y+box.h-15,100,30);
  confirm = UI::TextButton("Confirm", box.x+(box.w/2)-100-50,box.y+box.h-15,100,30); //not used!!!

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

void SpecifyTurnPicker::setCardinal(char c)
{
  cardinal = c;
  attack_specifier.setCardinal(c);
  defend_specifier.setCardinal(c);
  message_specifier.setCardinal(c);
  sabotage_specifier.setCardinal(c);
  scout_specifier.setCardinal(c);
}

SpecifyRequest SpecifyTurnPicker::touch(In &in)
{
  SpecifyRequest s;
  s.type = SpecifyRequest::NONE;
  s.action = action;

  if(in.type == In::DOWN)
  {
    if(cancel.query(in))
      s.type = SpecifyRequest::CANCEL_SPECIFY;
    else
    {
      switch(action->what)
      {
        case 'a':
          s.type = attack_specifier.touch(in).type;
          break;
        case 'd':
          s.type = defend_specifier.touch(in).type;
          break;
        case 'm':
          s.type = message_specifier.touch(in).type;
          break;
        case 's':
          s.type = sabotage_specifier.touch(in).type;
          break;
        case 'c':
          s.type = scout_specifier.touch(in).type;
          break;
        default:
          break;
      }
    }
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
      action_image.sprite = Sprite::telescope;
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

void SpecifyTurnPicker::setPower(int p)
{
  if(p > 0) power_0.sprite = Sprite::bolt; else power_0.sprite = Sprite::bolt_empty;
  if(p > 1) power_1.sprite = Sprite::bolt; else power_1.sprite = Sprite::bolt_empty;
}

void SpecifyTurnPicker::deactivate()
{

}

void SpecifyTurnPicker::activate()
{

}

