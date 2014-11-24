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

  backButton = UI::Button(10,10,40,40);

  hostGameLabel = UI::Label(ww/2-200,wh/2-200,40,"Host Game");

  ipLabel = UI::Label(ww/2-200,wh/2-160,40,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+60,wh/2-160,40,"8080");

  sessionButton  = UI::TextButton(ww/2-200, wh/2-100, 400, 40, "Start Session");

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
    SCENE_CHANGE_HACK = -1;
  }
  if(sessionButton.query(in))
  {
    if(!server) { server = new Network::Server(); *serverPtr = server; }
    if(server->con_state == Network::CONNECTION_STATE_DISCONNECTED) server->connect(8080);
  }
}

int HostScene::tick()
{
  if(server)
  {
    if(server->con_state == Network::CONNECTION_STATE_CONNECTED)
    {
      if(!s_model)
      {
        s_model = new ServerModel(server);
        *s_model_ptr = s_model;
      }
      if(!client) { client = new Network::Client(); *clientPtr = client; }
      if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED) client->connect(String("localhost"),8080);
    }
    if(server->con_state == Network::CONNECTION_STATE_STALE)
      server->disconnect();
  }
  if(client)
  {
    if(client->con_state == Network::CONNECTION_STATE_CONNECTED)
    {
      c_model = new ClientModel(client);
      *c_model_ptr = c_model;
      SCENE_CHANGE_HACK = 2;
    }
    if(client->con_state == Network::CONNECTION_STATE_STALE)
      client->disconnect();
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
  if(client) { if(client->con_state == Network::CONNECTION_STATE_CONNECTED) client->disconnect(); delete client; client = 0; *clientPtr = 0; }
  if(server) { if(server->con_state == Network::CONNECTION_STATE_CONNECTED) server->disconnect(); delete server; server = 0; *serverPtr = 0; }
}

HostScene::~HostScene()
{
}

