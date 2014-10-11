#include "host_scene.h"
#include "graphics.h"
#include "network.h"

#include <SDL.h>

#include "logger.h"

HostScene::HostScene(Graphics *g) : 
  hostGame("Host Game"),
  startSess("Start Session"),
  cancelSess("Cancel Session"),
  joinSess("Join Session"),
  leaveSess("Leave Session"),
  n('N'),
  s('S'),
  w('W'),
  e('E'),
  inRoom("In Room")
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  hostGameLabel = UI::Label(ww/2-100,wh/2-100,20,hostGame,10);

  Network::getIP(ip, &iplen);
  ipLabel = UI::Label(ww/2-100,wh/2-100,20,ip,iplen);

  port[0] = '8';port[1] = '0';port[2] = '8';port[3] = '0';
  portlen = 4;
  portLabel = UI::Label(ww/2-100,wh/2-100,20,port,portlen);

  nLabel = UI::Label(ww/2-10,     10, 20, &n, 1);  nButton = UI::Button(ww/2-10,     10, 20, 20);
  sLabel = UI::Label(ww/2-10,   wh-30, 20, &s, 1);  sButton = UI::Button(ww/2-10,   wh-30, 20, 20);
  wLabel = UI::Label(10,     wh/2-10, 20, &w, 1);  wButton = UI::Button(10,     wh/2-10, 20, 20);
  eLabel = UI::Label(ww-30,   wh/2-10, 20, &e, 1);  eButton = UI::Button(ww-30,   wh/2-10, 20, 20);

  startSessLabel  = UI::Label(ww/2-100, wh/2-50, 20, startSess,  14);
  cancelSessLabel = UI::Label(ww/2-100, wh/2-50, 20, cancelSess, 15);
  joinSessLabel   = UI::Label(ww/2-100, wh/2-50, 20, joinSess,   13);
  leaveSessLabel  = UI::Label(ww/2-100, wh/2-50, 20, leaveSess,  14);
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  inRoomLabel = UI::Label(ww/2-100, wh/2, 20, inRoom, 8);
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
  portLabel.draw(graphics);
  nLabel.draw(graphics);
  nButton.draw(graphics);
  sLabel.draw(graphics);
  sButton.draw(graphics);
  wLabel.draw(graphics);
  wButton.draw(graphics);
  eLabel.draw(graphics);
  eButton.draw(graphics);

  startSessLabel.draw(graphics);
  cancelSessLabel.draw(graphics);
  joinSessLabel.draw(graphics);
  leaveSessLabel.draw(graphics);
  sessionButton.draw(graphics);

  inRoomLabel.draw(graphics);
}

HostScene::~HostScene()
{

}

