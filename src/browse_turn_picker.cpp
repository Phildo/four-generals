#include "browse_turn_picker.h"
#include "graphics.h"
#include "input.h"
#include "logger.h"

void formButtonWithinRect(BrowseButton *b)
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

void BrowseButton::drawAtOffset(int o, Graphics *g)
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

  b.draw(g);
  i.draw(g);
  p0.draw(g);
  p1.draw(g);
  t.draw(g);
  d.draw(g);
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

  formButtonWithinRect(&attack);
  formButtonWithinRect(&defend);
  formButtonWithinRect(&message);
  formButtonWithinRect(&sabotage);
  formButtonWithinRect(&scout);

  attack.image.sprite = Sprite::sword;
  defend.image.sprite = Sprite::shield;
  message.image.sprite = Sprite::envelope;
  sabotage.image.sprite = Sprite::knife;
  scout.image.sprite = Sprite::knife;

  attack.title.text = String("Attack");
  defend.title.text = String("Defend");
  message.title.text = String("Message");
  sabotage.title.text = String("Sabotage");
  scout.title.text = String("Scout");

  attack.description.text = String("Attack is good");
  defend.description.text = String("Defend is good");
  message.description.text = String("Message is good");
  sabotage.description.text = String("Sabotage is good");
  scout.description.text = String("Scout is good");

  message.power_1.sprite = Sprite::bolt_empty;
  sabotage.power_1.sprite = Sprite::bolt_empty;
  scout.power_1.sprite = Sprite::bolt_empty;

  cancel  = UI::TextButton("Cancel", box.x+(box.w/2)-100-50,box.y+box.h-15,100,30);
}

BrowseTurnPicker::~BrowseTurnPicker()
{
}

void BrowseTurnPicker::touch(In &in)
{
  scroll.touch(in);
  if(in.type == In::UP && scroll.down_time < 10)
  {
    In i = in; //copy for mutation
    i.y -= scroll.offset;
    if(attack.button.query(i)) fg_log("Attack");
    if(defend.button.query(i)) fg_log("defend");
    if(message.button.query(i)) fg_log("message");
    if(sabotage.button.query(i)) fg_log("sabotage");
    if(scout.button.query(i)) fg_log("scout");
  }
}

void BrowseTurnPicker::tick()
{
  scroll.tick();
}

void BrowseTurnPicker::draw(Graphics *g)
{
  box.draw(g);
  titleLabel.draw(g);
  power_0.draw(g);
  power_1.draw(g);

  scroll.draw(g);
  attack.drawAtOffset(scroll.offset,g);
  defend.drawAtOffset(scroll.offset,g);
  message.drawAtOffset(scroll.offset,g);
  sabotage.drawAtOffset(scroll.offset,g);
  scout.drawAtOffset(scroll.offset,g);

  cancel.draw(g);
}

