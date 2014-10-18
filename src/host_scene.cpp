#include "host_scene.h"
#include "graphics.h"
#include "network.h"

#include <SDL.h>

#include "logger.h"

HostScene::HostScene(Graphics *g, Network::Server *& s, Network::Client *& c)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  hostGameLabel = UI::Label(ww/2-100,wh/2-100,20,"Host Game");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+30,wh/2-80,20,"8080");

  startSessLabel = UI::Label(ww/2-100, wh/2-50, 20, "Start Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  serverPtr = &s;
  clientPtr = &c;
  server = NULL;
  client = NULL;

  SCENE_CHANGE_HACK = 0;
}

void HostScene::touch(In &in)
{
  if(backButton.query(in))
  {
    fg_log("backButton");
    SCENE_CHANGE_HACK = -1;
  }
  if(sessionButton.query(in))
  {
    fg_log("sessionButton");
    if(!server) { server = new Network::Server(); *serverPtr = server; }
    if(!server->healthy()) server->connect(8080);
  }
}

int HostScene::tick()
{
  if(server && server->healthy())
  {
    if(!client) { client = new Network::Client(); *clientPtr = client; }
    if(!client->healthy()) client->connect(String("localhost"),8080);
  }
  if(client && client->healthy()) SCENE_CHANGE_HACK = 2;

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void HostScene::draw()
{
  backButton.draw(graphics);
  hostGameLabel.draw(graphics);
  ipLabel.draw(graphics);
  portLabel.draw(graphics);

  startSessLabel.draw(graphics);
  sessionButton.draw(graphics);
}

HostScene::~HostScene()
{
}

