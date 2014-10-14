#include "client_scene.h"
#include "graphics.h"
#include "network.h"

#include <SDL.h>

#include "logger.h"

ClientScene::ClientScene(Graphics *g, Network::Client *& c)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  joinGameLabel = UI::Label(ww/2-100,wh/2-100,20,"Join Game");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,Network::getIP().ptr());

  portLabel = UI::Label(ww/2-100,wh/2-100,20,"8080");

  joinSessLabel   = UI::Label(ww/2-100, wh/2-50, 20, "Join Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  client = 0;
}


void ClientScene::touch(In &in)
{
  if(backButton.query(in)) fg_log("backButton");
  if(sessionButton.query(in))
  {
    fg_log("sessionButton");
    client = new Network::Client();
    client->connect(String("localhost"),8080);
  }
}

int ClientScene::tick()
{
  return 0;
}

void ClientScene::draw()
{
  backButton.draw(graphics);
  joinGameLabel.draw(graphics);
  ipLabel.draw(graphics);
  portLabel.draw(graphics);

  joinSessLabel.draw(graphics);
  sessionButton.draw(graphics);
}

ClientScene::~ClientScene()
{
  if(client) delete client;
}

