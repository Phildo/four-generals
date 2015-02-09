#include "browse_turn_picker.h"
#include "graphics.h"
#include "input.h"
#include "logger.h"

void formButtonWithInMask(BrowseButton *b)
{
  b->image.rect = b->button.rect;
  b->image.rect.x += 10;
  b->image.rect.y += 10;
  b->image.rect.h -= 20;
  b->image.rect.w = b->image.rect.h;

  b->power_0 = UI::Image(Sprite::bolt, b->image.rect.rect);
  b->power_0.rect.x = b->power_0.rect.x+b->power_0.rect.w-60;
  b->power_0.rect.y = b->power_0.rect.y+b->power_0.rect.h-40;
  b->power_0.rect.w = 30;
  b->power_0.rect.h = 30;

  b->power_1 = UI::Image(Sprite::bolt, b->image.rect.rect);
  b->power_1.rect.x = b->power_1.rect.x+b->power_1.rect.w-40;
  b->power_1.rect.y = b->power_1.rect.y+b->power_1.rect.h-40;
  b->power_1.rect.w = 30;
  b->power_1.rect.h = 30;

  b->title = UI::Label("",b->image.rect.rect);
  b->title.rect.x = b->image.rect.x+b->image.rect.w+10;
  b->title.rect.h = 30;
  b->title.rect.w = b->button.rect.w-b->image.rect.w-20;

  b->description = UI::Label("",b->image.rect.rect);
  b->description.rect.x = b->image.rect.x+b->image.rect.w+10;
  b->description.rect.y = b->title.rect.y+b->title.rect.h+10;
  b->description.rect.h = 30;
  b->description.rect.w = b->button.rect.w-b->image.rect.w-20;
}

void BrowseButton::drawAtOffsetInMask(int o, Graphics *g, SDL_Rect r, bool enabled)
{
  UI::Button b = button;
  UI::Image i = image;
  UI::Image p0 = power_0;
  UI::Image p1 = power_1;
  UI::Label t = title;
  UI::Label d = description;

  b.rect.y += o;
  i.rect.y += o;
  p0.rect.y += o;
  p1.rect.y += o;
  t.rect.y += o;
  d.rect.y += o;

  b.drawInMask(g,r);
  i.drawInMask(g,r);
  p0.drawInMask(g,r);
  p1.drawInMask(g,r);
  t.drawInMask(g,r);
  d.drawInMask(g,r);

  if(!enabled) g->drawInMask(Sprite::black_a, b.rect.rect, r);
}

BrowseTurnPicker::BrowseTurnPicker()
{

}

BrowseTurnPicker::BrowseTurnPicker(Turn *t, UI::Box b)
{
  turn = t;

  box = b;
  titleLabel = UI::Label("Your Move:",box.x+10,box.y+10,34);
  power_0 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40-5-40, box.y+10, 40,40);
  power_1 = UI::Image(Sprite::bolt_empty, box.x+box.w-10-40     , box.y+10, 40,40);

  int buttonH = 110;

  SDL_Rect r = box.rect;
  r.x += 10;
  r.y += 40;
  r.w -= 20;
  r.h -= 50;
  SDL_Rect c = r;
  c.h = buttonH*5;
  scroll = UI::Scroller(r,c);

  attack.button   = UI::Button(scroll.rect.x,scroll.rect.y+buttonH*0,scroll.rect.w,buttonH);
  defend.button   = UI::Button(scroll.rect.x,scroll.rect.y+buttonH*1,scroll.rect.w,buttonH);
  message.button  = UI::Button(scroll.rect.x,scroll.rect.y+buttonH*2,scroll.rect.w,buttonH);
  sabotage.button = UI::Button(scroll.rect.x,scroll.rect.y+buttonH*3,scroll.rect.w,buttonH);
  scout.button    = UI::Button(scroll.rect.x,scroll.rect.y+buttonH*4,scroll.rect.w,buttonH);

  formButtonWithInMask(&attack);
  formButtonWithInMask(&defend);
  formButtonWithInMask(&message);
  formButtonWithInMask(&sabotage);
  formButtonWithInMask(&scout);

  attack.image.sprite = Sprite::sword;
  defend.image.sprite = Sprite::shield;
  message.image.sprite = Sprite::envelope;
  sabotage.image.sprite = Sprite::knife;
  scout.image.sprite = Sprite::knife;

  attack.title.text = String("Attack");
  defend.title.text = String("Fortify");
  message.title.text = String("Message");
  sabotage.title.text = String("Sabotage");
  scout.title.text = String("Scout");

  attack.description.text = String("Initiate an Attack");
  defend.description.text = String("Increase Defense and Retaliate");
  message.description.text = String("Schedule an Attack");
  sabotage.description.text = String("Impede enemy communication");
  scout.description.text = String("See all actions on the board");

  message.power_1.sprite = Sprite::bolt_empty;
  sabotage.power_1.sprite = Sprite::bolt_empty;
  scout.power_1.sprite = Sprite::bolt_empty;

  cancel = UI::TextButton("Cancel", box.x+(box.w/2)-50,box.y+box.h-15,100,30);

  listening = false;
}

BrowseTurnPicker::~BrowseTurnPicker()
{
}

BrowseRequest BrowseTurnPicker::touch(In &in)
{
  BrowseRequest b;
  b.type = BrowseRequest::NONE;

  if(in.type == In::DOWN) listening = true; //needs to see explicit "DOWN" before it starts responding
  if(!listening) return b;

  if(in.type == In::DOWN && cancel.query(in))
  {
    b.type = BrowseRequest::CANCEL_BROWSE;
    b.action = action;
    return b;
  }

  scroll.touch(in);
  if(in.type == In::UP && scroll.motion < 10)
  {
    In i = in; //copy for mutation
    i.y -= scroll.offset;

    b.type = BrowseRequest::SPECIFY_ACTION;
    b.action = action;

         if(attack.button.query(i)   && (turn->actions[0].power() == 0 && attackEnabled)) action->what = 'a';
    else if(defend.button.query(i)   && (turn->actions[0].power() == 0)) action->what = 'd';
    else if(message.button.query(i)  && (turn->actions[0].what != 'm'))  action->what = 'm';
    else if(sabotage.button.query(i) && (turn->actions[0].what != 's'))  action->what = 's';
    else if(scout.button.query(i)    && (turn->actions[0].what != 'c'))  action->what = 'c';
    else b.type = BrowseRequest::NONE;
  }

  return b;
}

void BrowseTurnPicker::tick()
{
  scroll.tick();
}

void BrowseTurnPicker::draw(Graphics *g)
{
  box.draw(g);

  scroll.draw(g);
  attack.drawAtOffsetInMask(  scroll.offset,g,scroll.rect.rect,(turn->actions[0].power() == 0 && attackEnabled));
  defend.drawAtOffsetInMask(  scroll.offset,g,scroll.rect.rect,(turn->actions[0].power() == 0));
  message.drawAtOffsetInMask( scroll.offset,g,scroll.rect.rect,(turn->actions[0].what != 'm'));
  sabotage.drawAtOffsetInMask(scroll.offset,g,scroll.rect.rect,(turn->actions[0].what != 's'));
  scout.drawAtOffsetInMask(   scroll.offset,g,scroll.rect.rect,(turn->actions[0].what != 'c'));

  titleLabel.draw(g);
  power_0.draw(g);
  power_1.draw(g);

  cancel.draw(g);
}

void BrowseTurnPicker::setAction(Action *a)
{
  action = a;
}

void BrowseTurnPicker::setPower(int p)
{
  if(p > 0) power_0.sprite = Sprite::bolt; else power_0.sprite = Sprite::bolt_empty;
  if(p > 1) power_1.sprite = Sprite::bolt; else power_1.sprite = Sprite::bolt_empty;
}

void BrowseTurnPicker::setAttackEnabled(bool e)
{
  attackEnabled = e;
}

void BrowseTurnPicker::deactivate()
{
  listening = false;
}

void BrowseTurnPicker::activate()
{
  //listening stays false until it sees an explicit 'DOWN' event
}

