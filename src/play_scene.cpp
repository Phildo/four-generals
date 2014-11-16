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

  dayLbls[Week::iday('s')] = UI::Label(space(ww,60,40,7,0), 20, 40, "Su");
  dayLbls[Week::iday('m')] = UI::Label(space(ww,60,40,7,1), 20, 40, "Mo");
  dayLbls[Week::iday('t')] = UI::Label(space(ww,60,40,7,2), 20, 40, "Tu");
  dayLbls[Week::iday('w')] = UI::Label(space(ww,60,40,7,3), 20, 40, "We");
  dayLbls[Week::iday('h')] = UI::Label(space(ww,60,40,7,4), 20, 40, "Th");
  dayLbls[Week::iday('f')] = UI::Label(space(ww,60,40,7,5), 20, 40, "Fr");
  dayLbls[Week::iday('a')] = UI::Label(space(ww,60,40,7,6), 20, 40, "Sa");

  dayBoxs[Week::iday('s')] = UI::Box(space(ww,60,40,7,0), 20, 40, 40);
  dayBoxs[Week::iday('m')] = UI::Box(space(ww,60,40,7,1), 20, 40, 40);
  dayBoxs[Week::iday('t')] = UI::Box(space(ww,60,40,7,2), 20, 40, 40);
  dayBoxs[Week::iday('w')] = UI::Box(space(ww,60,40,7,3), 20, 40, 40);
  dayBoxs[Week::iday('h')] = UI::Box(space(ww,60,40,7,4), 20, 40, 40);
  dayBoxs[Week::iday('f')] = UI::Box(space(ww,60,40,7,5), 20, 40, 40);
  dayBoxs[Week::iday('a')] = UI::Box(space(ww,60,40,7,6), 20, 40, 40);

  whenBtns[Week::iday('s')] = UI::Button(space(ww,60,40,7,0), 20, 40, 40);
  whenBtns[Week::iday('m')] = UI::Button(space(ww,60,40,7,1), 20, 40, 40);
  whenBtns[Week::iday('t')] = UI::Button(space(ww,60,40,7,2), 20, 40, 40);
  whenBtns[Week::iday('w')] = UI::Button(space(ww,60,40,7,3), 20, 40, 40);
  whenBtns[Week::iday('h')] = UI::Button(space(ww,60,40,7,4), 20, 40, 40);
  whenBtns[Week::iday('f')] = UI::Button(space(ww,60,40,7,5), 20, 40, 40);
  whenBtns[Week::iday('a')] = UI::Button(space(ww,60,40,7,6), 20, 40, 40);

  actionAttackButton   = UI::TextButton(space(ww,60,200,4,0), wh-60, 200, 40, "attack");
  actionMessageButton  = UI::TextButton(space(ww,60,200,4,1), wh-60, 200, 40, "message");
  actionDefendButton   = UI::TextButton(space(ww,60,200,4,2), wh-60, 200, 40, "defend");
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

  howLabel     = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "how");
  whichLabel   = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "which");
  whatLabel    = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "what");
  whoLabel     = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "who");
  whenLabel    = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "when");
  whereLabel   = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "where");
  messageLabel = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "message");
  debreifLabel = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "debreif");
  waitingLabel = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "waiting");
  winLabel     = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "WIN");
  loseLabel    = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "LOSE");

  confirmButton = UI::TextButton(space(ww,30,200,2,0), wh-60, 200, 40, "confirm");
  cancelButton  = UI::TextButton(space(ww,30,200,2,1), wh-60, 200, 40, "cancel");

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
  int xs[] = {ww/2-20,   ww-60, ww/2-20,      20};
  int ys[] = {    120, wh/2-20,  wh-160, wh/2-20};
  char scard[2]; scard[1] = '\0'; //holder to pass string to labels

  int icard;
  char card;

  //bottom (you)
  card = c->myCardinal();
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[2],ys[2], 40, scard);
  whoBtns[icard]   = UI::Button(xs[2],ys[2], 40, 40);
  whereBtns[icard] = UI::Button(xs[2],ys[2], 40, 40);

  //left (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[3],ys[3], 40, scard);
  whoBtns[icard]   = UI::Button(xs[3],ys[3], 40, 40);
  whereBtns[icard] = UI::Button(xs[3],ys[3], 40, 40);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[0],ys[0], 40, scard);
  whoBtns[icard]   = UI::Button(xs[0],ys[0], 40, 40);
  whereBtns[icard] = UI::Button(xs[0],ys[0], 40, 40);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  scard[0] = card;
  cardLbls[icard]  = UI::Label( xs[1],ys[1], 40, scard);
  whoBtns[icard]   = UI::Button(xs[1],ys[1], 40, 40);
  whereBtns[icard] = UI::Button(xs[1],ys[1], 40, 40);

  youBox = UI::Box(ww/2-20, wh-160, 40, 40);
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
  if(actionMessageButton.query(in)) { e.action = 'm'; e.to = Compass::opcardinal(e.cardinal); } //auto assign 'to'
  if(actionDefendButton.query(in))    e.action = 'd';
  if(actionSabotageButton.query(in))  e.action = 's';
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
    if(whereBtns[i].query(in)) e.where = Compass::cardinal(i);
}

void PlayScene::seekConfirmation(In &in)
{
  if(confirmButton.query(in)) client->broadcast(&e);
  if(cancelButton.query(in)) zeroE();
}

void PlayScene::drawAction()
{
  actionAttackButton.draw(graphics);
  actionMessageButton.draw(graphics);
  actionDefendButton.draw(graphics);
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

