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

#define pos(x) (10+((ww-20)/7)*x)+((ww-20)/7)/2-10

  sunLabel = UI::Label(pos(0), 10, 20, "Su");
  monLabel = UI::Label(pos(1), 10, 20, "Mo");
  tueLabel = UI::Label(pos(2), 10, 20, "Tu");
  wedLabel = UI::Label(pos(3), 10, 20, "We");
  thuLabel = UI::Label(pos(4), 10, 20, "Th");
  friLabel = UI::Label(pos(5), 10, 20, "Fr");
  satLabel = UI::Label(pos(6), 10, 20, "Sa");

  nLabel = UI::Label(ww/2-10,      10, 20, "N");
  sLabel = UI::Label(ww/2-10, wh  -30, 20, "S");
  wLabel = UI::Label(     10, wh/2-10, 20, "W");
  eLabel = UI::Label(ww  -30, wh/2-10, 20, "E");

  attackLabel = UI::Label(       20, wh-30, 20, "attack"); attackButton = UI::Button(       20, wh-30, 100, 20);
  defendLabel = UI::Label(ww-100-20, wh-30, 20, "defend"); defendButton = UI::Button(ww-100-20, wh-30, 100, 20);
}

void PlayScene::enter()
{
  client = *client_ptr;
  s_model = *s_model_ptr;
  c_model = *c_model_ptr;
}

void PlayScene::touch(In &in)
{
  if(backButton.query(in)) { }
  if(attackButton.query(in)) { }
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

  sunLabel.draw(graphics);
  monLabel.draw(graphics);
  tueLabel.draw(graphics);
  wedLabel.draw(graphics);
  thuLabel.draw(graphics);
  friLabel.draw(graphics);
  satLabel.draw(graphics);

  nLabel.draw(graphics);
  sLabel.draw(graphics);
  wLabel.draw(graphics);
  eLabel.draw(graphics);

  attackLabel.draw(graphics); attackButton.draw(graphics);
  defendLabel.draw(graphics); defendButton.draw(graphics);
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

