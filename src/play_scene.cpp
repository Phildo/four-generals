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

  generals_s[0] = Sprite::n_general();
  generals_s[1] = Sprite::e_general();
  generals_s[2] = Sprite::s_general();
  generals_s[3] = Sprite::w_general();
  force_field_s   = Sprite::force_field();
  shield_full_s   = Sprite::shield_full();
  shield_broken_s = Sprite::shield_broken();
  sword_s         = Sprite::sword();
  red_x_s         = Sprite::red_x();
  envelope_s      = Sprite::envelope();
  sun_s           = Sprite::sun();

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //(clockwise)           top                right              bottom(you)          left
  int posRectW[] = {               120,                120,                120,                120};
  int posRectH[] = {               120,                120,                120,                120};
  int posRectX[] = {ww/2-posRectW[0]/2,  ww-posRectW[1]-20, ww/2-posRectW[2]/2,                 20};
  int posRectY[] = {               120, wh/2-posRectH[1]/2,  wh-posRectH[2]-80, wh/2-posRectH[3]/2};

  for(int i = 0; i < 4; i++)
  {
    positionRects[i].x = posRectX[i];
    positionRects[i].y = posRectY[i];
    positionRects[i].w = posRectW[i];
    positionRects[i].h = posRectH[i];

    positionStatusRects[i].x = posRectX[i]+(posRectW[i]/2)+(posRectW[i]/4);
    positionStatusRects[i].y = posRectY[i]-(posRectH[i]/4);
    positionStatusRects[i].w = posRectW[i]/2;
    positionStatusRects[i].h = posRectH[i]/2;

    positionHealthRects[i].x = positionStatusRects[i].x;
    positionHealthRects[i].y = positionStatusRects[i].y+(2*positionStatusRects[i].h);
    positionHealthRects[i].w = positionStatusRects[i].w;
    positionHealthRects[i].h = positionStatusRects[i].h;
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

  dayLbls[0] = UI::Label(dayRects[0], "Su");
  dayLbls[1] = UI::Label(dayRects[1], "Mo");
  dayLbls[2] = UI::Label(dayRects[2], "Tu");
  dayLbls[3] = UI::Label(dayRects[3], "We");
  dayLbls[4] = UI::Label(dayRects[4], "Th");
  dayLbls[5] = UI::Label(dayRects[5], "Fr");
  dayLbls[6] = UI::Label(dayRects[6], "Sa");

  for(int i = 0; i < 7; i++)
    whenBtns[i] = UI::Button(dayRects[i]);

  actionAttackButton   = UI::TextButton(space(ww,60,200,4,0), wh-60, 200, 40, "attack");
  actionDefendButton   = UI::TextButton(space(ww,60,200,4,1), wh-60, 200, 40, "defend");
  actionMessageButton  = UI::TextButton(space(ww,60,200,4,2), wh-60, 200, 40, "message");
  actionSabotageButton = UI::TextButton(space(ww,60,200,4,3), wh-60, 200, 40, "sabotage");

  howBlockButton    = UI::TextButton(space(ww,60,200,3,0), wh-60, 200, 40, "Block");
  howReadButton     = UI::TextButton(space(ww,60,200,3,1), wh-60, 200, 40, "Read");
  howSabotageButton = UI::TextButton(space(ww,60,200,3,2), wh-60, 200, 40, "Sabotage");

  whichWhatButton  = UI::TextButton(space(ww,60,200,4,0), wh-60, 200, 40, "What");
  whichWhoButton   = UI::TextButton(space(ww,60,200,4,1), wh-60, 200, 40, "Who");
  whichWhenButton  = UI::TextButton(space(ww,60,200,4,2), wh-60, 200, 40, "When");
  whichWhereButton = UI::TextButton(space(ww,60,200,4,3), wh-60, 200, 40, "Where");

  whatAttackButton = UI::TextButton(space(ww,60,200,2,0), wh-60, 200, 40, "attack");
  whatDefendButton = UI::TextButton(space(ww,60,200,2,1), wh-60, 200, 40, "defend");

  howLabel      = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "how");
  whichLabel    = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "which");
  whatLabel     = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "what");
  whoLabel      = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "who");
  whenLabel     = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "when");
  whereLabel    = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "where");
  debreifLabel  = UI::Label(space(ww,0,600,1,0), wh/2-20, 30, "debreif");
  waitingLabel  = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "waiting");
  messageLabel  = UI::Label(space(ww,0,600,1,0), wh/2+20, 30, "message");
  sabotageLabel = UI::Label(space(ww,0,600,1,0), wh/2+50, 30, "sabotage");
  winLabel      = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "WIN");
  loseLabel     = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "LOSE");
  tieLabel      = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "TIE");

  sunBtn = UI::Button(dayRects[0]);
  sunDragging = false;

  confirmButton = UI::TextButton(space(ww,30,200,2,0), wh-60, 200, 40, "confirm");
  cancelButton  = UI::TextButton(space(ww,30,200,2,1), wh-60, 200, 40, "cancel");
  resetButton   = UI::TextButton(space(ww,30,200,1,0), wh-60, 200, 40, "reset game");

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
  cardImgs[icard]  = UI::Image(generals_s[icard], rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);

  //left (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('w');
  cardImgs[icard]  = UI::Image(generals_s[icard], rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('n');
  cardImgs[icard]  = UI::Image(generals_s[icard], rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('e');
  cardImgs[icard]  = UI::Image(generals_s[icard], rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);

  zeroE();
}

void PlayScene::zeroE()
{
  e.zero();
  e.connection = c->myConnection();
  e.cardinal = c->myCardinal();
  e.type = e_type_commit_action;
}

void PlayScene::chooseAction(In &in)
{
  if(actionAttackButton.query(in))    e.action = 'a';
  if(actionDefendButton.query(in))    e.action = 'd';
  if(actionMessageButton.query(in)) { e.action = 'm'; e.to = Compass::opcardinal(e.cardinal); } //auto assign 'to'
  if(actionSabotageButton.query(in))  e.action = 's';
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

void PlayScene::chooseHow(In &in)
{
  if(howBlockButton.query(in))    e.how = 'b';
  if(howReadButton.query(in))     e.how = 'r';
  if(howSabotageButton.query(in)) e.how = 's';
}

void PlayScene::chooseWhich(In &in)
{
  if(whichWhatButton.query(in))  e.which = 'a';
  if(whichWhoButton.query(in))   e.which = 'o';
  if(whichWhenButton.query(in))  e.which = 'e';
  if(whichWhereButton.query(in)) e.which = 'r';
}

void PlayScene::chooseWhat(In &in)
{
  if(whatAttackButton.query(in)) e.what = 'a';
  if(whatDefendButton.query(in)) e.what = 'd';
}

void PlayScene::chooseWho(In &in)
{
  for(int i = 0; i < 4; i++)
    if(whoBtns[i].query(in)) e.who = Compass::cardinal(i);
}

void PlayScene::chooseWhen(In &in)
{
  for(int i = 0; i < 7; i++)
    if(whenBtns[i].query(in)) e.when = Week::day(i);
}

void PlayScene::chooseWhere(In &in)
{
  for(int i = 0; i < 4; i++)
  {
    if(whereBtns[i].query(in))
    {
      if(Compass::cardinal(i) == Compass::cwcardinal( c->myCardinal()) ||
         Compass::cardinal(i) == Compass::ccwcardinal(c->myCardinal()))
        e.where = Compass::cardinal(i);
    }
  }
}

void PlayScene::seekConfirmation(In &in)
{
  if(confirmButton.query(in)) c->commitAction(e);
  if(cancelButton.query(in)) zeroE();
}

void PlayScene::drawAction()
{
  actionAttackButton.draw(graphics);
  actionDefendButton.draw(graphics);
  actionMessageButton.draw(graphics);
  actionSabotageButton.draw(graphics);
}

void PlayScene::drawHow()
{
  howLabel.draw(graphics);
  howBlockButton.draw(graphics);
  howReadButton.draw(graphics);
  howSabotageButton.draw(graphics);
}

void PlayScene::drawWhich()
{
  whichLabel.draw(graphics);
  whichWhatButton.draw(graphics);
  whichWhoButton.draw(graphics);
  whichWhenButton.draw(graphics);
  whichWhereButton.draw(graphics);
}

void PlayScene::drawWhat()
{
  whatLabel.draw(graphics);
  whatAttackButton.draw(graphics);
  whatDefendButton.draw(graphics);
}

void PlayScene::drawWho()
{
  whoLabel.draw(graphics);
}

void PlayScene::drawWhen()
{
  whenLabel.draw(graphics);
}

void PlayScene::drawWhere()
{
  whereLabel.draw(graphics);
}

void PlayScene::drawConfirmation()
{
  confirmButton.draw(graphics);
  cancelButton.draw(graphics);
}

void PlayScene::drawDebreif()
{
  debreifLabel.text = e.humanAction();
  debreifLabel.draw(graphics);
}

void PlayScene::drawWaiting()
{
  waitingLabel.draw(graphics);
}

void PlayScene::drawMessage()
{
  messageLabel.text = c->myMessage().message();
  messageLabel.draw(graphics);
}

void PlayScene::drawSabotage()
{
  sabotageLabel.text = c->mySabotage().message();
  sabotageLabel.draw(graphics);
}

void PlayScene::drawWin()
{
  winLabel.draw(graphics);
  if(s) resetButton.draw(graphics);
}

void PlayScene::drawLose()
{
  loseLabel.draw(graphics);
  if(s) resetButton.draw(graphics);
}

void PlayScene::drawTie()
{
  tieLabel.draw(graphics);
  if(s) resetButton.draw(graphics);
}

void PlayScene::touch(In &in)
{
  if(in.type != In::DOWN)
  {
    //only non-"touch" handling is dragging of sun
    if(in.type == In::UP) sunDragging = false;
    else if(in.type == In::MOVE && !c->iWin() && !c->iLose() && !c->iTie() && !c->iHaveAction() && e.action == '0')
    {
      chooseShownDay(in);
    }
    return;
  }

  //oh god terrible tree traversal touch propagation
  if(!c->iWin() && !c->iLose() && !c->iTie())
  {
    if(!c->iHaveAction())
    {
      if(e.action == '0') chooseAction(in);
      else if(e.action == 'a') //attack
      {
        if(e.who == '0') chooseWho(in);
        else seekConfirmation(in);
      }
      else if(e.action == 'm') //message
      {
        if(e.what == '0') chooseWhat(in);
        else if(e.what == 'a') //message attack
        {
          if(e.who == '0') chooseWho(in);
          else if(e.when == '0') chooseWhen(in);
          else if(e.where == '0') chooseWhere(in);
          else seekConfirmation(in);
        }
        else if(e.what == 'd') //message defend
        {
          if(e.when == '0') chooseWhen(in);
          else if(e.where == '0') chooseWhere(in);
          else seekConfirmation(in);
        }
      }
      else if(e.action == 'd') //defend
        seekConfirmation(in);
      else if(e.action == 's') //sabotage
      {
        if(e.how == '0') chooseHow(in);
        else if(e.how == 'b') seekConfirmation(in);
        else if(e.how == 'r') seekConfirmation(in);
        else if(e.how == 's')
        {
          if(e.which == '0') chooseWhich(in);
          else if(e.which == 'a')
          {
            if(e.what == '0') chooseWhat(in);
            else seekConfirmation(in);
          }
          else if(e.which == 'o')
          {
            if(e.who == '0') chooseWho(in);
            else seekConfirmation(in);
          }
          else if(e.which == 'e')
          {
            if(e.when == '0') chooseWhen(in);
            else seekConfirmation(in);
          }
          else if(e.which == 'r')
          {
            if(e.where == '0') chooseWhere(in);
            else seekConfirmation(in);
          }
        }
      }
    }
  }
  else if(s && resetButton.query(in))
  {
    c->requestReset();
  }
}

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();

  psys.tick(0.01f);
  if(anim_day < c->model.days)
  {
    anim_day += 0.01f;
    shown_day = anim_day;
  }
  if(anim_day > c->model.days) anim_day = (float)c->model.days;

  if(known_day != c->model.currentDay())
  {
    if(c->model.days == -1) return -1; //game was reset- go back to room

    zeroE();
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
  psys.draw(graphics);

  float snapped_shown_day = snapToInt(shown_day);
  if(snapped_shown_day > c->model.days) snapped_shown_day = (float)c->model.days;
  int shown_prev_day = ((int)(snapped_shown_day+1.0f))-1; //round up, subtract 1
  float t = snapped_shown_day-((float)shown_prev_day);

  //draws cardinals and actions
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);

    cardImgs[i].draw(graphics);

    if(t != 0.0f)
    {
      Event e = c->model.cardinalDayAction(card, shown_prev_day);
      if(e.action == 'a') graphics->draw(sword_s, rectForTraversal(card,e.who,t));
      if(e.action == 'd') graphics->draw(shield_full_s, rectForExpansion(card,t));
      if(e.action == 's') graphics->draw(red_x_s, rectForExpansion(card,t));
      if(e.action == 'm') graphics->draw(envelope_s, rectForTraversal(card,e.where,t));
      e = c->model.cardinalDayAction(card, shown_prev_day-1);
      if(e.action == 'm')
      {
        char at = e.where;
        char to = e.to;
        e = c->model.cardinalDayAction(at, shown_prev_day-1);
        if(!(e.action == 's' && e.how == 'b'))
          graphics->draw(envelope_s, rectForTraversal(at,to,t));
      }
    }
    //graphics->draw(spriteForAction(c->model.cardinalDayAction(card, shown_prev_day).action), rectForCardinalStatus(card));
  }

  SDL_Rect sunr = rectForTransition(Week::day(shown_prev_day%7), Week::day((shown_prev_day+1)%7), t);
  graphics->draw(sun_s,sunr);
  sunBtn.box.rect = sunr;
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
    if(c->iHaveMessage())
      drawMessage();
    if(c->iHaveSabotage())
      drawSabotage();
    if(!c->iHaveAction())
    {
      drawDebreif();
      if(e.action == '0') drawAction();
      else if(e.action == 'a') //attack
      {
        if(e.who == '0') drawWho();
        else drawConfirmation();
      }
      else if(e.action == 'm') //message
      {
        if(e.what == '0') drawWhat();
        else if(e.what == 'a') //message attack
        {
          if(e.who == '0') drawWho();
          else if(e.when == '0') drawWhen();
          else if(e.where == '0') drawWhere();
          else drawConfirmation();
        }
        else if(e.what == 'd') //message defend
        {
          if(e.when == '0') drawWhen();
          else if(e.where == '0') drawWhere();
          else drawConfirmation();
        }
      }
      else if(e.action == 'd') //defend
        drawConfirmation();
      else if(e.action == 's') //sabotage
      {
        if(e.how == '0') drawHow();
        else if(e.how == 'b') drawConfirmation();
        else if(e.how == 'r') drawConfirmation();
        else if(e.how == 's')
        {
          if(e.which == '0') drawWhich();
          else if(e.which == 'a')
          {
            if(e.what == '0') drawWhat();
            else drawConfirmation();
          }
          else if(e.which == 'o')
          {
            if(e.who == '0') drawWho();
            else drawConfirmation();
          }
          else if(e.which == 'e')
          {
            if(e.when == '0') drawWhen();
            else drawConfirmation();
          }
          else if(e.which == 'r')
          {
            if(e.where == '0') drawWhere();
            else drawConfirmation();
          }
        }
      }
    }
    else drawWaiting(); //wait
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

SDL_Rect PlayScene::rectForPositionHealth(char c)
{
  return positionHealthRects[Compass::icardinal(c)];
}

SDL_Rect PlayScene::rectForCardinalHealth(char card)
{
  char me = c->myCardinal();
  while(me != 's')
  {
    me   = Compass::cwcardinal(me);
    card = Compass::cwcardinal(card);
  }
  return rectForPositionHealth(card);
}

SDL_Rect PlayScene::rectForPositionStatus(char c)
{
  return positionStatusRects[Compass::icardinal(c)];
}

SDL_Rect PlayScene::rectForCardinalStatus(char card)
{
  char me = c->myCardinal();
  while(me != 's')
  {
    me   = Compass::cwcardinal(me);
    card = Compass::cwcardinal(card);
  }
  return rectForPositionStatus(card);
}

SDL_Rect PlayScene::spriteForAction(char card)
{
  if(card == 'd') return force_field_s;
  if(card == 'a') return sword_s;
  if(card == 'm') return envelope_s;
  if(card == 's') return red_x_s;
  return red_x_s;
}

