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
  serverPtr = &s;
  clientPtr = &c;
  s_model_ptr = &sm;
  c_model_ptr = &cm;
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
}

int HostScene::tick()
{
  if(!server) { server = new Network::Server(); *serverPtr = server; }
  if(server->con_state == Network::CONNECTION_STATE_DISCONNECTED) server->connect(4040);
  if(server->con_state == Network::CONNECTION_STATE_CONNECTED)
  {
    if(!s_model) s_model = new ServerModel(server);
    *s_model_ptr = s_model;
    if(!client) { client = new Network::Client(); *clientPtr = client; }
    if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED) client->connect(String("localhost"),4040);
  }
  if(server->con_state == Network::CONNECTION_STATE_STALE)
    server->disconnect();
  if(client)
  {
    if(client->con_state == Network::CONNECTION_STATE_CONNECTED)
    {
      if(!c_model) c_model = new ClientModel(client);
      *c_model_ptr = c_model;
      if(!c_model->imCardinal('n'))
      {
        static int g1 = 10;
        g1--;
        if(g1 == 0)
        {
          c_model->requestCardinal('n');
          g1 = 10;
        }
        c_model->tick();
        s_model->tick();
      }
      else
      {
        static int g2 = 10;
        g2--;
        if(g2 == 0)
        {
          c_model->requestBeginPlay();
          g2 = 10;
        }
        c_model->tick();
        s_model->tick();
        if(c_model->model.days != -1)
        {
          Turn t;
          //mock up model
          t.cardinal = 's';
          t.actions[0].what = 'a';
          t.actions[0].who = 'n';
          c_model->model.assignCardinalTurn('e', t);
          s_model->model.assignCardinalTurn('e', t);
          c_model->model.assignCardinalTurn('w', t);
          s_model->model.assignCardinalTurn('w', t);
          t.actions[0].who = 'e';
          c_model->model.assignCardinalTurn('s', t);
          s_model->model.assignCardinalTurn('s', t);
          SCENE_CHANGE_HACK = 3;
        }
      }
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
}

void HostScene::leave()
{

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

