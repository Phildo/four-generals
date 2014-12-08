#include "join_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "client_model.h"

#include "logger.h"

#include "SDL.h"

JoinScene::JoinScene(Graphics *g, Network::Client *& c, ClientModel *& cm)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(20,20,40,40);

  joinGameLabel = UI::Label(ww/2-200,wh/2-200,40,"Join Game");

  ipInput = UI::TextBox(ww/2-200,wh/2-160,260,40);
  portLabel = UI::Label(ww/2+60,wh/2-160,40,"8080");

  sessionButton  = UI::TextButton(ww/2-200, wh/2-100, 400, 40,"Join Session");

  keyboard = UI::Keyboard(0,wh-200,ww,200);
  ipInput.setText(Network::getIP());

  client_ptr = &c;
  c_model_ptr = &cm;
  client = 0;
  c_model = 0;

  SCENE_CHANGE_HACK = 0;
}

void JoinScene::enter()
{
  client = *client_ptr;
  c_model = *c_model_ptr;
}

void JoinScene::touch(In &in)
{
  keyboard.touch(in);
  if(backButton.query(in)) SCENE_CHANGE_HACK = -2;
  if(sessionButton.query(in))
  {
    if(!client) { client = new Network::Client(); *client_ptr = client; }
    if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED) client->connect(ipInput.getText(),8080);
  }
}

int JoinScene::tick()
{
  if(client)
  {
    if(client->con_state == Network::CONNECTION_STATE_CONNECTED)
    {
      c_model = new ClientModel(client);
      *c_model_ptr = c_model;
      SCENE_CHANGE_HACK = 1;
    }
    if(client->con_state == Network::CONNECTION_STATE_STALE)
      client->disconnect();
  }
  else
  {
    char c = keyboard.poll();
    if(c == '<') ipInput.backspace();
    else if(c != 0) ipInput.input(c);
  }

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void JoinScene::draw()
{
  backButton.draw(graphics);
  joinGameLabel.draw(graphics);

  ipInput.draw(graphics);

  portLabel.draw(graphics);

  sessionButton.draw(graphics);

  keyboard.draw(graphics);
}

void JoinScene::leave()
{

}
void JoinScene::pass()
{

}
void JoinScene::pop()
{
  if(c_model) { delete c_model; c_model = 0; *c_model_ptr = 0; }
  if(client) { if(client->con_state == Network::CONNECTION_STATE_CONNECTED) client->disconnect(); delete client; client = 0; *client_ptr = 0; }
}


JoinScene::~JoinScene()
{
}

