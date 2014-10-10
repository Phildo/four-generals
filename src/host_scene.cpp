#include "host_scene.h"
#include "graphics.h"
#include "network.h"

#include <SDL.h>

#include "logger.h"

HostScene::HostScene(Graphics *g)
{
  graphics = g;
  Network::getIP(ip, &iplen);

  ipLabel  = UI::Label( (g->winWidth()/2)-100,(g->winHeight()/2)-10,20,ip,iplen);

  nLabel  = UI::Label( (g->winWidth()/2)-100,25,    20,"N",1);
  nButton = UI::Button((g->winWidth()/2)-100,25,200,20);

  sLabel  = UI::Label( (g->winWidth()/2)-100,g->winHeight()-25-20,    20,"S",1);
  sButton = UI::Button((g->winWidth()/2)-100,g->winHeight()-25-20,200,20);

  wLabel  = UI::Label( 25,(g->winHeight()/2)-10,      20,"W",1);
  wButton = UI::Button(25,(g->winHeight()/2)-10, 200, 20);

  eLabel  = UI::Label( g->winWidth()-25-200,(g->winHeight()/2)-10,      20,"E",1);
  eButton = UI::Button(g->winWidth()-25-200,(g->winHeight()/2)-10, 200, 20);

}


void HostScene::touch(In &in)
{
  if(nButton.query(in)) fg_log("touched n");
  if(sButton.query(in)) fg_log("touched s");
  if(wButton.query(in)) fg_log("touched w");
  if(eButton.query(in)) fg_log("touched e");
}

int HostScene::tick()
{
  return 0;
}

void HostScene::draw()
{
  ipLabel.draw(graphics);

  nLabel.draw(graphics);
  nButton.draw(graphics);

  sLabel.draw(graphics);
  sButton.draw(graphics);

  wLabel.draw(graphics);
  wButton.draw(graphics);

  eLabel.draw(graphics);
  eButton.draw(graphics);
}

HostScene::~HostScene()
{

}

