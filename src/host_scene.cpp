#include "host_scene.h"
#include "graphics.h"
#include "network.h"

#include <SDL.h>

#include "logger.h"

HostScene::HostScene(Graphics *g)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  hostGameLabel = UI::Label(ww/2-100,wh/2-100,20,"Host Game");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,"exampleip");

  portLabel = UI::Label(ww/2-100,wh/2-100,20,"8080");

  nLabel = UI::Label(ww/2-10,      10, 20, "N");  nButton = UI::Button(ww/2-10,     10, 20, 20);
  sLabel = UI::Label(ww/2-10, wh  -30, 20, "S");  sButton = UI::Button(ww/2-10,   wh-30, 20, 20);
  wLabel = UI::Label(10,      wh/2-10, 20, "W");  wButton = UI::Button(10,     wh/2-10, 20, 20);
  eLabel = UI::Label(ww-30,   wh/2-10, 20, "E");  eButton = UI::Button(ww-30,   wh/2-10, 20, 20);

  startSessLabel  = UI::Label(ww/2-100, wh/2-50, 20, "Start Session");
  cancelSessLabel = UI::Label(ww/2-100, wh/2-50, 20, "Cancel Session");
  joinSessLabel   = UI::Label(ww/2-100, wh/2-50, 20, "Join Session");
  leaveSessLabel  = UI::Label(ww/2-100, wh/2-50, 20, "Leave Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  inRoomLabel = UI::Label(ww/2-100, wh/2, 20, "In Room");
}


void HostScene::touch(In &in)
{
  if(backButton.query(in)) fg_log("backButton");
  if(nButton.query(in)) fg_log("nButton");
  if(sButton.query(in)) fg_log("sButton");
  if(wButton.query(in)) fg_log("wButton");
  if(eButton.query(in)) fg_log("eButton");
  if(sessionButton.query(in)) fg_log("sessionButton");
}

int HostScene::tick()
{
  return 0;
}

void HostScene::draw()
{
  backButton.draw(graphics);
  hostGameLabel.draw(graphics);
  ipLabel.draw(graphics);
  //portLabel.draw(graphics);

  nLabel.draw(graphics);
  sLabel.draw(graphics);
  wLabel.draw(graphics);
  eLabel.draw(graphics);

  nButton.draw(graphics);
  sButton.draw(graphics);
  wButton.draw(graphics);
  eButton.draw(graphics);

  startSessLabel.draw(graphics);
  //cancelSessLabel.draw(graphics);
  //joinSessLabel.draw(graphics);
  //leaveSessLabel.draw(graphics);
  sessionButton.draw(graphics);

  inRoomLabel.draw(graphics);
}

HostScene::~HostScene()
{

}

