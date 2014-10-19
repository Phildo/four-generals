#include "room_scene.h"
#include "graphics.h"
#include "network.h"
#include "model.h"

#include <SDL.h>

#include "logger.h"

RoomScene::RoomScene(Graphics *g, Network::Client *& c, Model *&m)
{
  graphics = g;
  model = m;

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

  clientPtr = &c;
  client = 0;

  modelPtr = &m;
  model = 0;
}

void RoomScene::enter()
{
}

void RoomScene::touch(In &in)
{
  if(backButton.query(in)) fg_log("backButton");
  if(nButton.query(in)) fg_log("nButton");
  if(sButton.query(in)) fg_log("sButton");
  if(wButton.query(in)) fg_log("wButton");
  if(eButton.query(in)) fg_log("eButton");
  if(sessionButton.query(in)) { fg_log("sessionButton"); }
}

int RoomScene::tick()
{
  return 0;
}

void RoomScene::draw()
{
  backButton.draw(graphics);
  inRoomLabel.draw(graphics);
  ipLabel.draw(graphics);
  portLabel.draw(graphics);

  nLabel.draw(graphics);
  sLabel.draw(graphics);
  wLabel.draw(graphics);
  eLabel.draw(graphics);

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

RoomScene::~RoomScene()
{

}

