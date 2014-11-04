#include "play_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

PlayScene::PlayScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  client_ptr = &c;
  s_model_ptr = &sm;
  c_model_ptr = &cm;
  client = 0;
  s_model = 0;
  c_model = 0;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

#define pos(x) (30+((ww-60)/7)*x)+((ww-60)/7)/2-10

  sunLabel = UI::Label(pos(0), 10, 20, "Su"); sunBox = UI::Box(pos(0), 10, 20, 20); whenSunButton = UI::Button(pos(0), 10, 20, 20);
  monLabel = UI::Label(pos(1), 10, 20, "Mo"); monBox = UI::Box(pos(1), 10, 20, 20); whenMonButton = UI::Button(pos(1), 10, 20, 20);
  tueLabel = UI::Label(pos(2), 10, 20, "Tu"); tueBox = UI::Box(pos(2), 10, 20, 20); whenTueButton = UI::Button(pos(2), 10, 20, 20);
  wedLabel = UI::Label(pos(3), 10, 20, "We"); wedBox = UI::Box(pos(3), 10, 20, 20); whenWedButton = UI::Button(pos(3), 10, 20, 20);
  thuLabel = UI::Label(pos(4), 10, 20, "Th"); thuBox = UI::Box(pos(4), 10, 20, 20); whenThuButton = UI::Button(pos(4), 10, 20, 20);
  friLabel = UI::Label(pos(5), 10, 20, "Fr"); friBox = UI::Box(pos(5), 10, 20, 20); whenFriButton = UI::Button(pos(5), 10, 20, 20);
  satLabel = UI::Label(pos(6), 10, 20, "Sa"); satBox = UI::Box(pos(6), 10, 20, 20); whenSatButton = UI::Button(pos(6), 10, 20, 20);

  actionAttackLabel  = UI::Label(         20, wh-30, 20, "attack");  actionAttackButton  = UI::Button(       20, wh-30, 100, 20); whatAttackButton = UI::Button(       20, wh-30, 100, 20); whatAttackLabel = UI::Label(       20, wh-30, 20, "attack");
  actionMessageLabel = UI::Label(ww  -100-20, wh-30, 20, "message"); actionMessageButton = UI::Button(ww-100-20, wh-30, 100, 20);
  actionDefendLabel  = UI::Label(ww/2-50    , wh-30, 20, "defend");  actionDefendButton  = UI::Button(ww/2-50  , wh-30, 100, 20); whatDefendButton = UI::Button(ww/2-50  , wh-30, 100, 20); whatDefendLabel = UI::Label(ww/2-50  , wh-30, 20, "defend");

  whatLabel    = UI::Label(ww/2-50, wh/2-10, 20, "what");
  whoLabel     = UI::Label(ww/2-50, wh/2-10, 20, "who");
  whenLabel    = UI::Label(ww/2-50, wh/2-10, 20, "when");
  whereLabel   = UI::Label(ww/2-50, wh/2-10, 20, "where");
  debreifLabel = UI::Label(ww/2-50, wh/2-10, 20, "debreif");
  waitingLabel = UI::Label(ww/2-50, wh/2-10, 20, "waiting");

  confirmLabel = UI::Label(         20, wh-30, 20, "confirm"); confirmButton = UI::Button(       20, wh-30, 100, 20);
  cancelLabel  = UI::Label(ww/2-50    , wh-30, 20, "cancel");  cancelButton  = UI::Button(ww/2-50  , wh-30, 100, 20);
}

void PlayScene::enter()
{
  client = *client_ptr;
  s_model = *s_model_ptr;
  c_model = *c_model_ptr;

  //need to wait until c_model is obtained
  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //             n        e     s(you)        w
  int xs[] = {ww/2-10,   ww-30, ww/2-10,      10};
  int ys[] = {     60, wh/2-10,   wh-80, wh/2-10};
  int card = 0;
  if(c_model->cardGeneral('n')->connection == client->connection) card = 2;
  if(c_model->cardGeneral('e')->connection == client->connection) card = 3;
  if(c_model->cardGeneral('s')->connection == client->connection) card = 0;
  if(c_model->cardGeneral('w')->connection == client->connection) card = 1;
  nLabel = UI::Label(xs[(card+0)%4],ys[(card+0)%4], 20, "n"); whoNButton = UI::Button(xs[(card+0)%4],ys[(card+0)%4], 20, 20); whereNButton = UI::Button(xs[(card+0)%4],ys[(card+0)%4], 20, 20);
  eLabel = UI::Label(xs[(card+1)%4],ys[(card+1)%4], 20, "e"); whoEButton = UI::Button(xs[(card+1)%4],ys[(card+1)%4], 20, 20); whereEButton = UI::Button(xs[(card+1)%4],ys[(card+1)%4], 20, 20);
  sLabel = UI::Label(xs[(card+2)%4],ys[(card+2)%4], 20, "s"); whoSButton = UI::Button(xs[(card+2)%4],ys[(card+2)%4], 20, 20); whereSButton = UI::Button(xs[(card+2)%4],ys[(card+2)%4], 20, 20);
  wLabel = UI::Label(xs[(card+3)%4],ys[(card+3)%4], 20, "w"); whoWButton = UI::Button(xs[(card+3)%4],ys[(card+3)%4], 20, 20); whereWButton = UI::Button(xs[(card+3)%4],ys[(card+3)%4], 20, 20);

  youBox = UI::Box(ww/2-10, wh-80, 20, 20);

  e.zero(); e.connection = client->connection; e.cardinal = c_model->conGeneral(client->connection)->cardinal; e.type = Network::e_type_commit_action;
}

void PlayScene::touch(In &in)
{
  if(backButton.query(in)) { }

  //oh god terrible tree traversal touch propagation

  if(c_model->conAction(client->connection)->type == '0') //hasn't had confirmation of action
  {
    //choose action
    if(e.action == '0')
    {
      if(actionAttackButton.query(in))  { e.action = 'a'; }
      if(actionMessageButton.query(in)) { e.action = 'm'; }
      if(actionDefendButton.query(in))  { e.action = 'd'; }
    }
    else if(e.action == 'a')
    {
      //choose who
      if(e.who == '0')
      {
        if(whoNButton.query(in)) e.who = 'n';
        if(whoEButton.query(in)) e.who = 'e';
        if(whoSButton.query(in)) e.who = 's';
        if(whoWButton.query(in)) e.who = 'w';
      }
      else
      {
        //seek confirmation
        if(confirmButton.query(in)) client->broadcast(e);
        if(cancelButton.query(in)) { e.zero(); e.connection = client->connection; e.cardinal = c_model->conGeneral(client->connection)->cardinal; e.type = Network::e_type_commit_action; }
      }
    }
    else if(e.action == 'm')
    {
      //choose what
      if(e.what == '0')
      {
        if(whatAttackButton.query(in)) e.what = 'a';
        if(whatDefendButton.query(in)) e.what = 'd';
      }
      else if(e.what == 'a')
      {
        //choose who
        if(e.who == '0')
        {
          if(whoNButton.query(in)) e.who = 'n';
          if(whoEButton.query(in)) e.who = 'e';
          if(whoSButton.query(in)) e.who = 's';
          if(whoWButton.query(in)) e.who = 'w';
        }
        else
        {
          //choose when
          if(e.when == '0')
          {
            if(whenSunButton.query(in)) e.when = 's';
            if(whenMonButton.query(in)) e.when = 'm';
            if(whenTueButton.query(in)) e.when = 't';
            if(whenWedButton.query(in)) e.when = 'w';
            if(whenThuButton.query(in)) e.when = 'h';
            if(whenFriButton.query(in)) e.when = 'f';
            if(whenSatButton.query(in)) e.when = 'a';
          }
          else
          {
            //choose where
            if(e.where == '0')
            {
              if(whereNButton.query(in)) e.where = 'n';
              if(whereEButton.query(in)) e.where = 'e';
              if(whereSButton.query(in)) e.where = 's';
              if(whereWButton.query(in)) e.where = 'w';
            }
            else
            {
              //seek confirmation
              if(confirmButton.query(in)) client->broadcast(e);
              if(cancelButton.query(in)) { e.zero(); e.connection = client->connection; e.cardinal = c_model->conGeneral(client->connection)->cardinal; e.type = Network::e_type_commit_action;  }
            }
          }
        }
      }
      else if(e.what == 'd')
      {
        //choose when
        if(e.when == '0')
        {
          if(whenSunButton.query(in)) e.when = 's';
          if(whenMonButton.query(in)) e.when = 'm';
          if(whenTueButton.query(in)) e.when = 't';
          if(whenWedButton.query(in)) e.when = 'w';
          if(whenThuButton.query(in)) e.when = 'h';
          if(whenFriButton.query(in)) e.when = 'f';
          if(whenSatButton.query(in)) e.when = 'a';
        }
        else
        {
          //choose where
          if(e.where == '0')
          {
            if(whereNButton.query(in)) e.where = 'n';
            if(whereEButton.query(in)) e.where = 'e';
            if(whereSButton.query(in)) e.where = 's';
            if(whereWButton.query(in)) e.where = 'w';
          }
          else
          {
            //seek confirmation
            if(confirmButton.query(in)) client->broadcast(e);
            if(cancelButton.query(in)) { e.zero(); e.connection = client->connection; e.cardinal = c_model->conGeneral(client->connection)->cardinal; e.type = Network::e_type_commit_action;   }
          }
        }
      }
    }
    else if(e.action == 'd')
    {
        //seek confirmation
        if(confirmButton.query(in)) client->broadcast(e);
        if(cancelButton.query(in)) { e.zero(); e.connection = client->connection; e.cardinal = c_model->conGeneral(client->connection)->cardinal; e.type = Network::e_type_commit_action;    }
    }
  }
}

int PlayScene::tick()
{
  if(s_model) s_model->tick();
  c_model->tick();
  return 0;
}

void PlayScene::draw()
{
  backButton.draw(graphics);

  sunLabel.draw(graphics); if(c_model->day == 's') sunBox.draw(graphics);
  monLabel.draw(graphics); if(c_model->day == 'm') monBox.draw(graphics);
  tueLabel.draw(graphics); if(c_model->day == 't') tueBox.draw(graphics);
  wedLabel.draw(graphics); if(c_model->day == 'w') wedBox.draw(graphics);
  thuLabel.draw(graphics); if(c_model->day == 'h') thuBox.draw(graphics);
  friLabel.draw(graphics); if(c_model->day == 'f') friBox.draw(graphics);
  satLabel.draw(graphics); if(c_model->day == 'a') satBox.draw(graphics);

  nLabel.draw(graphics);
  sLabel.draw(graphics);
  wLabel.draw(graphics);
  eLabel.draw(graphics);
  youBox.draw(graphics);

  if(c_model->conAction(client->connection)->type == '0') //hasn't had confirmation of action
  {
    if(e.action == '0')
    {
      actionAttackButton.draw(graphics);  actionAttackLabel.draw(graphics);
      actionMessageButton.draw(graphics); actionMessageLabel.draw(graphics);
      actionDefendButton.draw(graphics);  actionDefendLabel.draw(graphics);
    }
    else if(e.action == 'a')
    {
      if(e.who == '0')
        whoLabel.draw(graphics);
      else
      {
        confirmButton.draw(graphics); confirmLabel.draw(graphics);
        cancelButton.draw(graphics);  cancelLabel.draw(graphics);
      }
    }
    else if(e.action == 'm')
    {
      if(e.what == '0')
      {
        whatLabel.draw(graphics);
        whatAttackButton.draw(graphics); whatAttackLabel.draw(graphics);
        whatDefendButton.draw(graphics); whatDefendLabel.draw(graphics);
      }
      else if(e.what == 'a')
      {
        if(e.who == '0')
          whoLabel.draw(graphics);
        else
        {
          if(e.when == '0')
            whenLabel.draw(graphics);
          else
          {
            if(e.where == '0')
              whereLabel.draw(graphics);
            else
            {
              confirmButton.draw(graphics); confirmLabel.draw(graphics);
              cancelButton.draw(graphics);  cancelLabel.draw(graphics);
            }
          }
        }
      }
      else if(e.what == 'd')
      {
        if(e.when == '0')
          whenLabel.draw(graphics);
        else
        {
          if(e.where == '0')
            whereLabel.draw(graphics);
          else
          {
            confirmButton.draw(graphics); confirmLabel.draw(graphics);
            cancelButton.draw(graphics);  cancelLabel.draw(graphics);
          }
        }
      }
    }
    else if(e.action == 'd')
    {
      confirmButton.draw(graphics); confirmLabel.draw(graphics);
      cancelButton.draw(graphics);  cancelLabel.draw(graphics);
    }
  }
  else
    waitingLabel.draw(graphics);
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

