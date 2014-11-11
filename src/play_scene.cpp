#include "play_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

#include "compass.h"
#include "week.h"

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

  backButton = UI::Button(10,10,20,20);

#define pos(x) (30+((ww-60)/7)*x)+((ww-60)/7)/2-10

  dayLbls[Week::iday('s')] = UI::Label(pos(0), 10, 20, "Su");
  dayLbls[Week::iday('m')] = UI::Label(pos(1), 10, 20, "Mo");
  dayLbls[Week::iday('t')] = UI::Label(pos(2), 10, 20, "Tu");
  dayLbls[Week::iday('w')] = UI::Label(pos(3), 10, 20, "We");
  dayLbls[Week::iday('h')] = UI::Label(pos(4), 10, 20, "Th");
  dayLbls[Week::iday('f')] = UI::Label(pos(5), 10, 20, "Fr");
  dayLbls[Week::iday('a')] = UI::Label(pos(6), 10, 20, "Sa");

  dayBoxs[Week::iday('s')] = UI::Box(pos(0), 10, 20, 20);
  dayBoxs[Week::iday('m')] = UI::Box(pos(1), 10, 20, 20);
  dayBoxs[Week::iday('t')] = UI::Box(pos(2), 10, 20, 20);
  dayBoxs[Week::iday('w')] = UI::Box(pos(3), 10, 20, 20);
  dayBoxs[Week::iday('h')] = UI::Box(pos(4), 10, 20, 20);
  dayBoxs[Week::iday('f')] = UI::Box(pos(5), 10, 20, 20);
  dayBoxs[Week::iday('a')] = UI::Box(pos(6), 10, 20, 20);

  whenBtns[Week::iday('s')] = UI::Button(pos(0), 10, 20, 20);
  whenBtns[Week::iday('m')] = UI::Button(pos(1), 10, 20, 20);
  whenBtns[Week::iday('t')] = UI::Button(pos(2), 10, 20, 20);
  whenBtns[Week::iday('w')] = UI::Button(pos(3), 10, 20, 20);
  whenBtns[Week::iday('h')] = UI::Button(pos(4), 10, 20, 20);
  whenBtns[Week::iday('f')] = UI::Button(pos(5), 10, 20, 20);
  whenBtns[Week::iday('a')] = UI::Button(pos(6), 10, 20, 20);

  actionAttackLabel  = UI::Label(         20, wh-30, 20, "attack");  actionAttackButton  = UI::Button(       20, wh-30, 100, 20); whatAttackButton = UI::Button(       20, wh-30, 100, 20); whatAttackLabel = UI::Label(       20, wh-30, 20, "attack");
  actionMessageLabel = UI::Label(ww  -100-20, wh-30, 20, "message"); actionMessageButton = UI::Button(ww-100-20, wh-30, 100, 20);
  actionDefendLabel  = UI::Label(ww/2-50    , wh-30, 20, "defend");  actionDefendButton  = UI::Button(ww/2-50  , wh-30, 100, 20); whatDefendButton = UI::Button(ww/2-50  , wh-30, 100, 20); whatDefendLabel = UI::Label(ww/2-50  , wh-30, 20, "defend");

  whatLabel    = UI::Label(ww/2-50, wh/2-10, 20, "what");
  whoLabel     = UI::Label(ww/2-50, wh/2-10, 20, "who");
  whenLabel    = UI::Label(ww/2-50, wh/2-10, 20, "when");
  whereLabel   = UI::Label(ww/2-50, wh/2-10, 20, "where");
  messageLabel = UI::Label(ww/2-50, wh/2-10, 20, "message");
  debreifLabel = UI::Label(ww/2-50, wh/2-10, 20, "debreif");
  waitingLabel = UI::Label(ww/2-50, wh/2-10, 20, "waiting");
  winLabel     = UI::Label(ww/2-50, wh/2-10, 20, "WIN");
  loseLabel    = UI::Label(ww/2-50, wh/2-10, 20, "LOSE");

  confirmLabel = UI::Label(         20, wh-30, 20, "confirm"); confirmButton = UI::Button(       20, wh-30, 100, 20);
  cancelLabel  = UI::Label(ww/2-50    , wh-30, 20, "cancel");  cancelButton  = UI::Button(ww/2-50  , wh-30, 100, 20);

  known_day = '0';
}

void PlayScene::enter()
{
  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;

  //need to wait until c_model is obtained
  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //(clockwise) top     right  bottom(you) left
  int xs[] = {ww/2-10,   ww-30, ww/2-10,      10};
  int ys[] = {     60, wh/2-10,   wh-80, wh/2-10};
  char scard[2]; scard[1] = '\0'; //holder to pass string to labels

  int icard;
  char card;

  //bottom (you)
  card = c->myCardinal();
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[2],ys[2], 20, scard);
  whoBtns[icard]   = UI::Button(xs[2],ys[2], 20, 20);
  whereBtns[icard] = UI::Button(xs[2],ys[2], 20, 20);

  //left (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[3],ys[3], 20, scard);
  whoBtns[icard]   = UI::Button(xs[3],ys[3], 20, 20);
  whereBtns[icard] = UI::Button(xs[3],ys[3], 20, 20);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[0],ys[0], 20, scard);
  whoBtns[icard]   = UI::Button(xs[0],ys[0], 20, 20);
  whereBtns[icard] = UI::Button(xs[0],ys[0], 20, 20);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[1],ys[1], 20, scard);
  whoBtns[icard]   = UI::Button(xs[1],ys[1], 20, 20);
  whereBtns[icard] = UI::Button(xs[1],ys[1], 20, 20);

  youBox = UI::Box(ww/2-10, wh-80, 20, 20);
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
  if(actionAttackButton.query(in))  e.action = 'a';
  if(actionMessageButton.query(in)) { e.action = 'm'; e.to = Compass::opcardinal(e.cardinal); } //auto assign 'to'
  if(actionDefendButton.query(in))  e.action = 'd';
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
    if(whereBtns[i].query(in)) e.where = Compass::cardinal(i);
}

void PlayScene::seekConfirmation(In &in)
{
  if(confirmButton.query(in)) client->broadcast(&e);
  if(cancelButton.query(in)) zeroE();
}

void PlayScene::drawAction()
{
  actionAttackButton.draw(graphics);  actionAttackLabel.draw(graphics);
  actionMessageButton.draw(graphics); actionMessageLabel.draw(graphics);
  actionDefendButton.draw(graphics);  actionDefendLabel.draw(graphics);
}

void PlayScene::drawWhat()
{
  whatLabel.draw(graphics);
  whatAttackButton.draw(graphics); whatAttackLabel.draw(graphics);
  whatDefendButton.draw(graphics); whatDefendLabel.draw(graphics);
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
  confirmButton.draw(graphics); confirmLabel.draw(graphics);
  cancelButton.draw(graphics);  cancelLabel.draw(graphics);
}

void PlayScene::drawDebreif()
{
  int ww = graphics->winWidth();
  int wh = graphics->winHeight();
  debreifLabel = UI::Label(ww/2-130, wh/2-30, 15, e.humanAction());
  debreifLabel.draw(graphics);
}

void PlayScene::drawWaiting()
{
  waitingLabel.draw(graphics);
}

void PlayScene::drawMessage()
{
  int ww = graphics->winWidth();
  int wh = graphics->winHeight();
  messageLabel = UI::Label(ww/2-130, wh/2+10, 15, c->myMessage().message());
  messageLabel.draw(graphics);
}

void PlayScene::drawWin()
{
  winLabel.draw(graphics);
}

void PlayScene::drawLose()
{
  loseLabel.draw(graphics);
}

void PlayScene::touch(In &in)
{
  if(backButton.query(in)) { }

  //oh god terrible tree traversal touch propagation

  if(!c->iWin() && !c->iLose())
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
    }
  }
}

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();
  if(known_day != c->model.currentDay())
  {
    zeroE();
    known_day = c->model.currentDay();
  }
  return 0;
}

void PlayScene::draw()
{
  backButton.draw(graphics);

  for(int i = 0; i < 7; i++)
  {
    dayLbls[i].draw(graphics);
    if(c->model.currentDay() == Week::day(i))
      dayBoxs[i].draw(graphics);
  }

  for(int i = 0; i < 4; i++)
    cardLbls[i].draw(graphics);
  youBox.draw(graphics);

  if(c->iWin())
  {
    drawWin();
  }
  else if(c->iLose())
  {
    drawLose();
  }
  else
  {
    if(c->iHaveMessage())
      drawMessage();
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
      else if(e.action == 'd') drawConfirmation();
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

