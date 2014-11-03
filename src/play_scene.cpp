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

  sunLabel = UI::Label(pos(0), 10, 20, "Su"); sunBox = UI::Box(pos(0), 10, 20, 20);
  monLabel = UI::Label(pos(1), 10, 20, "Mo"); monBox = UI::Box(pos(1), 10, 20, 20);
  tueLabel = UI::Label(pos(2), 10, 20, "Tu"); tueBox = UI::Box(pos(2), 10, 20, 20);
  wedLabel = UI::Label(pos(3), 10, 20, "We"); wedBox = UI::Box(pos(3), 10, 20, 20);
  thuLabel = UI::Label(pos(4), 10, 20, "Th"); thuBox = UI::Box(pos(4), 10, 20, 20);
  friLabel = UI::Label(pos(5), 10, 20, "Fr"); friBox = UI::Box(pos(5), 10, 20, 20);
  satLabel = UI::Label(pos(6), 10, 20, "Sa"); satBox = UI::Box(pos(6), 10, 20, 20);

  attackLabel  = UI::Label(         20, wh-30, 20, "attack");  attackButton  = UI::Button(       20, wh-30, 100, 20);
  messageLabel = UI::Label(ww  -100-20, wh-30, 20, "message"); messageButton = UI::Button(ww-100-20, wh-30, 100, 20);
  defendLabel  = UI::Label(ww/2-50    , wh-30, 20, "defend");  defendButton  = UI::Button(ww/2-50  , wh-30, 100, 20);

  youBox = UI::Box(ww/2-10, wh-80, 20, 20);
}

void PlayScene::enter()
{
  client = *client_ptr;
  s_model = *s_model_ptr;
  c_model = *c_model_ptr;

  //need to wait until c_model is obtained
  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  char cardinals[] = "N\0E\0S\0W\0";
  int card = 0;
  if(c_model->cardGeneral('n')->connection == client->connection) card = 0;
  if(c_model->cardGeneral('e')->connection == client->connection) card = 1;
  if(c_model->cardGeneral('s')->connection == client->connection) card = 2;
  if(c_model->cardGeneral('w')->connection == client->connection) card = 3;
  sLabel = UI::Label(ww/2-10, wh  -80, 20, &cardinals[((card+0)%4)*2]); //S //you
  wLabel = UI::Label(     10, wh/2-10, 20, &cardinals[((card+1)%4)*2]); //W
  nLabel = UI::Label(ww/2-10,      60, 20, &cardinals[((card+2)%4)*2]); //N
  eLabel = UI::Label(ww  -30, wh/2-10, 20, &cardinals[((card+3)%4)*2]); //E
}

void PlayScene::touch(In &in)
{
  if(backButton.query(in)) { }
  if(attackButton.query(in)) { }
  if(messageButton.query(in)) { }
  if(defendButton.query(in)) { }
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

  attackLabel.draw(graphics);  attackButton.draw(graphics);
  messageLabel.draw(graphics); messageButton.draw(graphics);
  defendLabel.draw(graphics);  defendButton.draw(graphics);
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

