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

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //Rects
  int ws[] = {          90,          110,          120,          110};
  int hs[] = {          90,          110,          120,          110};
  int xs[] = {ww/2-ws[0]/2,  ww-ws[1]-20, ww/2-ws[2]/2,           20};
  int ys[] = {          60, wh/2-hs[1]/2,  wh-hs[2]-40, wh/2-hs[3]/2};

  for(int i = 0; i < 4; i++)
  {
    posRects[i]      = UI::Box(xs[i],       ys[i], ws[i],   hs[i]).rect;
    posLabelRects[i] = UI::Box(xs[i], ys[i]+hs[i], ws[i], hs[i]/3).rect;
  }

  for(int i = 0; i < 7; i++)
  {
    dayRects[i] = UI::Box(space(ww,60,40,7,i), 20, 40, 40).rect;
    sunRects[i] = UI::Box(space(ww,50,60,7,i), 10, 60, 60).rect;
  }

  //Sprites
  generals_s[0] = UI::AnimSprites(Sprite::gen_n_0(), Sprite::gen_n_1(), Sprite::gen_n_2(), Sprite::gen_n_3());
  generals_s[1] = UI::AnimSprites(Sprite::gen_e_0(), Sprite::gen_e_1(), Sprite::gen_e_2(), Sprite::gen_e_3());
  generals_s[2] = UI::AnimSprites(Sprite::gen_s_0(), Sprite::gen_s_1(), Sprite::gen_s_2(), Sprite::gen_s_3());
  generals_s[3] = UI::AnimSprites(Sprite::gen_w_0(), Sprite::gen_w_1(), Sprite::gen_w_2(), Sprite::gen_w_3());

  pTags[0] = Sprite::p1();     pTags[1] = Sprite::p2();    pTags[2] = Sprite::p3();    pTags[3] = Sprite::p4();
  pTagsW[0] = Sprite::p1_w(); pTagsW[1] = Sprite::p2_w(); pTagsW[2] = Sprite::p3_w(); pTagsW[3] = Sprite::p4_w();
  pTagsB[0] = Sprite::p1_b(); pTagsB[1] = Sprite::p2_b(); pTagsB[2] = Sprite::p3_b(); pTagsB[3] = Sprite::p4_b();
  pTagsR[0] = Sprite::p1_r(); pTagsR[1] = Sprite::p2_r(); pTagsR[2] = Sprite::p3_r(); pTagsR[3] = Sprite::p4_r();

  //Hacks
  char daynamehacks[] = {'S','u','\0','M','o','\0','T','u','\0','W','e','\0','T','h','\0','F','r','\0','S','a','\0'};
  char cardnamehacks[] = {'N','o','r','t','h','\0','E','a','s','t','\0','\0','S','o','u','t','h','\0','W','e','s','t','\0','\0'};

  //Views
  for(int i = 0; i < 4; i++)
  {
    cardImgs[i] = UI::Anim(generals_s[i], 4, 1.f, posRects[i]);
    cardLbls[i] = UI::Label(&cardnamehacks[i*6], posLabelRects[i]);
  }

  for(int i = 0; i < 7; i++)
    dayLbls[i] = UI::Label(&daynamehacks[i*3], dayRects[i]);

  sunBtn = UI::Button(dayRects[0]);

  read_sabotage_0 = UI::ImageButtonRound(Sprite::red_x(),      10,wh-50,20,20);
  read_sabotage_1 = UI::ImageButtonRound(Sprite::red_x(),      10,wh-80,20,20);
  read_message    = UI::ImageButtonRound(Sprite::envelope(),ww-30,wh-50,20,20);

  loading = UI::Anim(UI::AnimSprites(Sprite::loading_0(), Sprite::loading_1(), Sprite::loading_2(), Sprite::loading_2()), 3, 1.f, ww/2-250, wh/2-120,40,40);
  waiting_on_players_label = UI::Label("waiting on players...", wh/2-20, 200, 40);
  reset_game_button        = UI::TextButton("reset game", ww/2-100, wh/2-20, 200, 40);

  win_img  = UI::Image(Sprite::sun(),   ww/2-100, wh/2-100, 200, 200);
  lose_img = UI::Image(Sprite::red_x(), ww/2-100, wh/2-100, 200, 200);
  tie_img  = UI::Image(Sprite::sun(),   ww/2-100, wh/2-100, 200, 200);

  //state
  known_day = '0';
  anim_day = 0.0f;
  shown_day = 0.0f;

  sunDragging = false;
  sabotage_0_reading = false;
  sabotage_1_reading = false;
  message_reading = false;
}

void PlayScene::enter()
{
  //need to wait until c_model is obtained
  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;

  //syncs position rects with assigned cardinal rects
  char card = c->myCardinal(); int ic = Compass::icardinal(card);
  char pos  = 's';             int ip = Compass::icardinal(pos);
  for(int i = 0; i < 4; i++)
  {
    cardRects[ic]      = posRects[ip];
    cardLabelRects[ic] = posLabelRects[ip];
    card = Compass::cwcardinal(card); ic = Compass::icardinal(card);
    pos  = Compass::cwcardinal(pos);  ip = Compass::icardinal(pos);
  }

  //places general images+labels
  for(int i = 0; i < 4; i++)
  {
    cardImgs[i] = UI::Anim(generals_s[i], 4, 1.f, cardRects[i]);
    cardLbls[i].rect = cardLabelRects[i];
  }
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

  if(in.type == In::DOWN)
  {
    if(!c->model.roundOver())
    {
      //pass to turn picker
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

  if(known_day != c->model.currentDay()) //turn passed
  {
    if(c->model.days == -1) return -1; //game was reset- go back to room

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
  graphics->draw(Sprite::sun(),sunr);
  sunBtn.rect.rect = sunr;
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(graphics);

  if(c->model.roundOver())
  {
         if(c->iWin())  drawWin();
    else if(c->iLose()) drawLose();
    else if(c->iTie())  drawTie();
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
    //draw wturn picker
    picker.draw(graphics);
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

SDL_Rect PlayScene::rectForTraversal(char fcard, char tcard, float t)
{
  return LerpRect::lerp(cardRects[Compass::icardinal(fcard)], cardRects[Compass::icardinal(tcard)], t);
}

SDL_Rect PlayScene::rectForExpansion(char card, float t)
{
  SDL_Rect tmp = cardRects[Compass::icardinal(card)];
  tmp.x -= tmp.w/2;
  tmp.y -= tmp.h/2;
  tmp.w *= 2;
  tmp.h *= 2;
  return LerpRect::lerp(cardRects[Compass::icardinal(card)], tmp, t);
}

SDL_Rect PlayScene::rectForTransition(char fd, char td, float t)
{
  return LerpRect::lerp(sunRects[Week::iday(fd)], sunRects[Week::iday(td)], t);
}

