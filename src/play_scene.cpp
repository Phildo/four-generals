#include "play_scene.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"
#include "week.h"

#include "SDL.h"

PlayScene::PlayScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  client_ptr = &c;
  s_ptr = &sm;
  c_ptr = &cm;
  client = 0;
  s = 0;
  c = 0;

  generals_s[0] = UI::AnimSprites(Sprite::gen_n_0(), Sprite::gen_n_1(), Sprite::gen_n_2(), Sprite::gen_n_3());
  generals_s[1] = UI::AnimSprites(Sprite::gen_e_0(), Sprite::gen_e_1(), Sprite::gen_e_2(), Sprite::gen_e_3());
  generals_s[2] = UI::AnimSprites(Sprite::gen_s_0(), Sprite::gen_s_1(), Sprite::gen_s_2(), Sprite::gen_s_3());
  generals_s[3] = UI::AnimSprites(Sprite::gen_w_0(), Sprite::gen_w_1(), Sprite::gen_w_2(), Sprite::gen_w_3());

  pTags[0] = Sprite::p1();
  pTags[1] = Sprite::p2();
  pTags[2] = Sprite::p3();
  pTags[3] = Sprite::p4();
  pTagsW[0] = Sprite::p1_w();
  pTagsW[1] = Sprite::p2_w();
  pTagsW[2] = Sprite::p3_w();
  pTagsW[3] = Sprite::p4_w();
  pTagsB[0] = Sprite::p1_b();
  pTagsB[1] = Sprite::p2_b();
  pTagsB[2] = Sprite::p3_b();
  pTagsB[3] = Sprite::p4_b();
  pTagsR[0] = Sprite::p1_r();
  pTagsR[1] = Sprite::p2_r();
  pTagsR[2] = Sprite::p3_r();
  pTagsR[3] = Sprite::p4_r();

  force_field_s   = Sprite::shield_full_force();
  shield_full_s   = Sprite::shield_full();
  shield_broken_s = Sprite::shield_cracked();
  sword_s         = Sprite::sword();
  red_x_s         = Sprite::red_x();
  envelope_s      = Sprite::envelope();
  sun_s           = Sprite::sun();
  sblock_s        = Sprite::sblock();
  sread_s         = Sprite::sread();
  sswitch_s       = Sprite::sswitch();
  what_s          = Sprite::what();
  who_ns_s        = Sprite::who_ns();
  who_we_s        = Sprite::who_we();
  when_s          = Sprite::when();
  where_ns_s      = Sprite::where_ns();
  where_we_s      = Sprite::where_we();
  null_s          = Sprite::null();

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  int posRectW[] = {                90,                110,                120,                110};
  int posRectH[] = {                90,                110,                120,                110};
  int posRectX[] = {ww/2-posRectW[0]/2,  ww-posRectW[1]-20, ww/2-posRectW[2]/2,                 20};
  int posRectY[] = {                60, wh/2-posRectH[1]/2,  wh-posRectH[2]-40, wh/2-posRectH[3]/2};

  for(int i = 0; i < 4; i++)
  {
    positionRects[i].x = posRectX[i];
    positionRects[i].y = posRectY[i];
    positionRects[i].w = posRectW[i];
    positionRects[i].h = posRectH[i];
  }

  for(int i = 0; i < 7; i++)
  {
    dayRects[i].x = space(ww,60,40,7,i);
    dayRects[i].y = 20;
    dayRects[i].w = 40;
    dayRects[i].h = 40;

    sunRects[i].x = space(ww,50,60,7,i);
    sunRects[i].y = 10;
    sunRects[i].w = 60;
    sunRects[i].h = 60;
  }

  dayLbls[0] = UI::Label("Su", dayRects[0]);
  dayLbls[1] = UI::Label("Mo", dayRects[1]);
  dayLbls[2] = UI::Label("Tu", dayRects[2]);
  dayLbls[3] = UI::Label("We", dayRects[3]);
  dayLbls[4] = UI::Label("Th", dayRects[4]);
  dayLbls[5] = UI::Label("Fr", dayRects[5]);
  dayLbls[6] = UI::Label("Sa", dayRects[6]);

  sunBtn = UI::Button(dayRects[0]);
  sunDragging = false;

  sabotage_0_reading = false;
  read_sabotage_0 = UI::ImageButtonRound(   red_x_s,   10,wh-50,20,20);
  sabotage_1_reading = false;
  read_sabotage_1 = UI::ImageButtonRound(   red_x_s,   10,wh-80,20,20);
  message_reading = false;
  read_message    = UI::ImageButtonRound(envelope_s,ww-30,wh-50,20,20);

  cardImgs[Compass::icardinal('n')] = UI::Anim(generals_s[Compass::icardinal('n')], 4, 1.f, rectForPosition('n'));
  cardImgs[Compass::icardinal('e')] = UI::Anim(generals_s[Compass::icardinal('e')], 4, 1.f, rectForPosition('n'));
  cardImgs[Compass::icardinal('s')] = UI::Anim(generals_s[Compass::icardinal('s')], 4, 1.f, rectForPosition('n'));
  cardImgs[Compass::icardinal('w')] = UI::Anim(generals_s[Compass::icardinal('w')], 4, 1.f, rectForPosition('n'));

  cardLbls[Compass::icardinal('n')] = UI::Label("north", rectForPosition('n'));
  cardLbls[Compass::icardinal('e')] = UI::Label("east",  rectForPosition('n'));
  cardLbls[Compass::icardinal('s')] = UI::Label("south", rectForPosition('n'));
  cardLbls[Compass::icardinal('w')] = UI::Label("west",  rectForPosition('n'));

  UI::TextButton cancel_single_button("cancel",  space(ww,200,100,1,0),350,100,30);
  UI::TextButton cancel_double_button("cancel",  space(ww,200,100,2,0),350,100,30);
  UI::TextButton confirm_double_button("confirm",space(ww,200,100,2,1),350,100,30);

  UI::Button whoButtonCW(rectForPosition('w'));
  UI::Button whoButtonCCW(rectForPosition('e'));

  loading = UI::Anim(UI::AnimSprites(Sprite::loading_0(), Sprite::loading_1(), Sprite::loading_2(), Sprite::loading_2()), 3, 1.f, ww/2-250, wh/2-120,40,40);
  waiting_on_players_label = UI::Label("waiting on players...", wh/2-20, 200, 40);
  reset_game_button        = UI::TextButton("reset game", ww/2-100, wh/2-20, 200, 40);
  win_img  = UI::Image(sun_s,   ww/2-100, wh/2-100, 200, 200);
  lose_img = UI::Image(red_x_s, ww/2-100, wh/2-100, 200, 200);
  tie_img  = UI::Image(sun_s,   ww/2-100, wh/2-100, 200, 200);

  known_day = '0';
  anim_day = 0.0f;
  shown_day = 0.0f;
}

void PlayScene::enter()
{
  //need to wait until c_model is obtained
  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;

  char card;
  int icard;
  SDL_Rect rect;

  //bottom (you)
  card = c->myCardinal();
  icard = Compass::icardinal(card);
  rect = rectForPosition('s');
  cardImgs[icard] = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;

  //left (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('w');
  cardImgs[icard]  = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('n');
  cardImgs[icard]  = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('e');
  cardImgs[icard]  = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;
}

void PlayScene::chooseShownDay(In &in)
{
  if(sunBtn.query(in)) sunDragging = true;

  if(sunDragging)
  {
    int firstX = dayRects[0].x+(dayRects[0].w/2);
    int lastX  = dayRects[6].x+(dayRects[6].w/2);
    shown_day = ((float)(in.x-firstX)/(float)(lastX-firstX))*6.0f;
  }
}

void PlayScene::chooseReadSabotage(In &in)
{
  if(read_sabotage_0.query(in)) sabotage_0_reading = true;
  if(read_sabotage_1.query(in)) sabotage_1_reading = true;
}

void PlayScene::chooseReadMessage(In &in)
{
  if(read_message.query(in)) message_reading = true;
}

void PlayScene::drawSabotage0()
{
  read_sabotage_0.draw(graphics);
}

void PlayScene::drawSabotage1()
{
  read_sabotage_1.draw(graphics);
}

void PlayScene::drawMessage()
{
  read_message.draw(graphics);
}

void PlayScene::drawWaiting()
{
  loading.draw(graphics);
  waiting_on_players_label.draw(graphics);
}

void PlayScene::drawReset()
{
  reset_game_button.draw(graphics);
}

void PlayScene::drawWin()
{
  win_img.draw(graphics);
}

void PlayScene::drawLose()
{
  lose_img.draw(graphics);
}

void PlayScene::drawTie()
{
  tie_img.draw(graphics);
}















void PlayScene::touch(In &in)
{
  if(in.type == In::UP)
  {
    sunDragging = false;
    sabotage_0_reading = false;
    sabotage_1_reading = false;
    message_reading = false;
  }

  if((in.type == In::DOWN || in.type == In::MOVE) && !c->iHaveTurn())
    chooseShownDay(in);

  if(in.type == In::DOWN)
  {
    Action a;
    Action a0;
    Action a1;
    if(c->myMessage(a))
    {
      chooseReadMessage(in);
    }
    if(c->mySabotage(a0, a1))
    {
      chooseReadSabotage(in);
    }
  }

  //oh god terrible tree traversal touch propagation
  if(in.type == In::DOWN)
  {
    if(!c->iWin() && !c->iLose() && !c->iTie())
    {
      if(!c->iHaveTurn()) { }
    }
    else if(s && reset_game_button.query(in))
    {
      c->requestReset();
    }
  }
}

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();

  for(int i = 0; i < 4; i++)
    cardImgs[i].tick(0.4f);

  if(anim_day < c->model.days)
  {
    anim_day += 0.01f;
    shown_day = anim_day;
  }
  if(anim_day > c->model.days) anim_day = (float)c->model.days;

  if(known_day != c->model.currentDay())
  {
    if(c->model.days == -1) return -1; //game was reset- go back to room

    t.zero();
    known_day = c->model.currentDay();
  }
  return 0;
}

float snapToInt(float v)
{
  float left = (float)((int)v);
  float right = (float)((int)(v+1.0f));
  if(v < left +0.01f) return left;
  if(v > right-0.01f) return right;
  return v;
}
void PlayScene::draw()
{
  float snapped_shown_day = snapToInt(shown_day);
  if(snapped_shown_day > c->model.days) snapped_shown_day = (float)c->model.days;
  if(snapped_shown_day < 0) snapped_shown_day = 0;
  int shown_prev_day = ((int)(snapped_shown_day+1.0f))-1; //round up, subtract 1
  float t = snapped_shown_day-((float)shown_prev_day);

  //draws cardinals and actions
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);

    cardImgs[i].draw(graphics);
    cardLbls[i].draw(graphics);

    if(t != 0.0f)
    {
    /*
      Action a = c->model.cardinalDayTurn(card, shown_prev_day);
      if(a.what == 'a') graphics->draw(sword_s, rectForTraversal(card,e.who,t));
      if(a.what == 'd') graphics->draw(shield_full_s, rectForExpansion(card,t));
      if(a.what == 's') graphics->draw(red_x_s, rectForExpansion(card,t));
      if(a.what == 'm') graphics->draw(envelope_s, rectForTraversal(card,e.route,t));
      a = c->model.cardinalDayTurn(card, shown_prev_day-1);
      if(a.what == 'm')
      {
        char at = a.route;
        char to = a.to;
        a = c->model.cardinalDayTurn(at, shown_prev_day-1);
        if(!(a.what == 's' && a.how == 'b'))
          graphics->draw(envelope_s, rectForTraversal(at,to,t));
      }
    */
    }
  }

  SDL_Rect sunr = rectForTransition(Week::day(shown_prev_day%7), Week::day((shown_prev_day+1)%7), t);
  graphics->draw(sun_s,sunr);
  sunBtn.rect.rect = sunr;
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(graphics);

  if(c->iWin())
  {
    drawWin();
  }
  else if(c->iLose())
  {
    drawLose();
  }
  else if(c->iTie())
  {
    drawTie();
  }
  else
  {
    Action a;
    Action a0;
    Action a1;
    if(c->myMessage(a))
    {
      drawMessage();
    }
    if(c->mySabotage(a0, a1))
    {
      if(a0.what != '0') drawSabotage0();
      if(a1.what != '0') drawSabotage1();
    }
    if(!c->iHaveTurn()) {}
    else drawWaiting();
  }
}

void PlayScene::leave()
{
}
void PlayScene::pass()
{

}
void PlayScene::pop()
{
}

PlayScene::~PlayScene()
{

}

SDL_Rect PlayScene::rectForPosition(char c)
{
  return positionRects[Compass::icardinal(c)];
}

SDL_Rect PlayScene::rectForCardinal(char card)
{
  char me = c->myCardinal();
  while(me != 's')
  {
    me   = Compass::cwcardinal(me);
    card = Compass::cwcardinal(card);
  }
  return rectForPosition(card);
}

SDL_Rect PlayScene::rectForDay(char d)
{
  return dayRects[Week::iday(d)];
}

SDL_Rect PlayScene::rectForSun(char d)
{
  return sunRects[Week::iday(d)];
}

SDL_Rect PlayScene::rectForTraversal(char fcard, char tcard, float t)
{
  return LerpRect::lerp(rectForCardinal(fcard), rectForCardinal(tcard), t);
}

SDL_Rect PlayScene::rectForExpansion(char card, float t)
{
  SDL_Rect tmp = rectForCardinal(card);
  tmp.x -= tmp.w/2;
  tmp.y -= tmp.h/2;
  tmp.w *= 2;
  tmp.h *= 2;
  return LerpRect::lerp(rectForCardinal(card), tmp, t);
  return tmp;
}

SDL_Rect PlayScene::rectForTransition(char fd, char td, float t)
{
  return LerpRect::lerp(rectForSun(fd), rectForSun(td), t);
}

