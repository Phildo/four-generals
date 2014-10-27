#include "room_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

RoomScene::RoomScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
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

  inRoomLabel = UI::Label(ww/2-100,wh/2-100,20,"In Room");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+30,wh/2-80,20,"8080");

  leaveSessLabel  = UI::Label(ww/2-100, wh/2-50, 20, "Leave Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  nLabel = UI::Label(ww/2-10,      10, 20, "N");  nButton = UI::Button(ww/2-10,      10, 20, 20);
  sLabel = UI::Label(ww/2-10, wh  -30, 20, "S");  sButton = UI::Button(ww/2-10, wh  -30, 20, 20);
  wLabel = UI::Label(     10, wh/2-10, 20, "W");  wButton = UI::Button(     10, wh/2-10, 20, 20);
  eLabel = UI::Label(ww  -30, wh/2-10, 20, "E");  eButton = UI::Button(ww  -30, wh/2-10, 20, 20);
}

void RoomScene::enter()
{
  client = *client_ptr;
  s_model = *s_model_ptr;
  c_model = *c_model_ptr;
}

void RoomScene::touch(In &in)
{
  if(backButton.query(in)) { }
  if(nButton.query(in)) { client->broadcast('n', Network::e_type_assign_card); }
  if(sButton.query(in)) { client->broadcast('s', Network::e_type_assign_card); }
  if(wButton.query(in)) { client->broadcast('w', Network::e_type_assign_card); }
  if(eButton.query(in)) { client->broadcast('e', Network::e_type_assign_card); }
  if(sessionButton.query(in)) { }
}

int RoomScene::tick()
{
  if(s_model) s_model->tick();
  c_model->tick();
  return 0;
}

void RoomScene::draw()
{
  backButton.draw(graphics);
  inRoomLabel.draw(graphics);
  ipLabel.draw(graphics);
  portLabel.draw(graphics);

  if(c_model->cardGeneral('n')) nLabel.draw(graphics);
  if(c_model->cardGeneral('s')) sLabel.draw(graphics);
  if(c_model->cardGeneral('w')) wLabel.draw(graphics);
  if(c_model->cardGeneral('e')) eLabel.draw(graphics);

  nButton.draw(graphics);
  sButton.draw(graphics);
  wButton.draw(graphics);
  eButton.draw(graphics);

  leaveSessLabel.draw(graphics);
  sessionButton.draw(graphics);
}

void RoomScene::leave()
{
}
void RoomScene::pass()
{

}
void RoomScene::pop()
{
}

RoomScene::~RoomScene()
{

}

