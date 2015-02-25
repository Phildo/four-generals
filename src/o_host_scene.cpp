#include "host_scene.h"
#include "graphics.h"
#include "network.h"
#include "input.h"
#include "server.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "SDL.h"

HostScene::HostScene(Graphics *g, Network::Server *& s, Network::Client *& c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  serverPtr = &s;
  clientPtr = &c;
  s_model_ptr = &sm;
  c_model_ptr = &cm;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,40,40);

  hostGameLabel = UI::Label("Host Game",ww/2-250, wh/2-170,40);
  ipLabel     = UI::Label(Network::getIP().ptr(),ww/2-250, wh/2-120,40);
  portLabel   = UI::Label("4040",ww/2+150, wh/2-120,40);

  sessionButton = UI::TextButton("Start Session",ww/2-250, wh/2-70, 500, 40);

  reset();
}

void HostScene::reset()
{
  server = 0;
  client = 0;
  s_model = 0;
  c_model = 0;

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
  if(in.type != In::DOWN) return;
  if(backButton.query(in))
  {
    SCENE_CHANGE_HACK = -1;
  }
  if(sessionButton.query(in))
  {
    if(!server) { server = new Network::Server(); *serverPtr = server; }
    if(server->con_state == Network::CONNECTION_STATE_DISCONNECTED) server->connect(4040);
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
      if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED) client->connect(String("localhost"),4040);
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
  reset();
}
void HostScene::pass()
{
}
void HostScene::pop()
{
}

HostScene::~HostScene()
{
}

