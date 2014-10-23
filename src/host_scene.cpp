#include "host_scene.h"
#include "graphics.h"
#include "network.h"
#include "server.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

HostScene::HostScene(Graphics *g, Network::Server *& s, Network::Client *& c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  serverPtr = &s;
  clientPtr = &c;
  s_model_ptr = &sm;
  c_model_ptr = &cm;
  server = 0;
  client = 0;
  s_model = 0;
  c_model = 0;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  hostGameLabel = UI::Label(ww/2-100,wh/2-100,20,"Host Game");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+30,wh/2-80,20,"8080");

  startSessLabel = UI::Label(ww/2-100, wh/2-50, 20, "Start Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  SCENE_CHANGE_HACK = 0;
}

void HostScene::enter()
{
  server = *serverPtr;
  client = *clientPtr;
  s_model = *s_model_ptr;
  c_model = *c_model_ptr;
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
    if(!s_model)
    {
      s_model = new ServerModel(server);
      *s_model_ptr = s_model;
    }
    if(!client) { client = new Network::Client(); *clientPtr = client; }
    if(!client->healthy()) client->connect(String("localhost"),8080);
  }
  if(client && client->healthy()) 
  {
    c_model = new ClientModel(client);
    *c_model_ptr = c_model;
    SCENE_CHANGE_HACK = 2;
  }

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

void HostScene::leave()
{

}
void HostScene::pass()
{

}
void HostScene::pop()
{
  if(c_model) { delete c_model; c_model = 0; *c_model_ptr = 0; }
  if(s_model) { delete s_model; s_model = 0; *s_model_ptr = 0; }
  if(client) { if(client->healthy()) client->disconnect(); delete client; client = 0; *clientPtr = 0; }
  if(server) { if(server->healthy()) server->disconnect(); delete server; server = 0; *serverPtr = 0; }
}

HostScene::~HostScene()
{
}

