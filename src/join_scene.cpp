#include "join_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

JoinScene::JoinScene(Graphics *g, Network::Client *& c, ClientModel *& cm)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  joinGameLabel = UI::Label(ww/2-100,wh/2-100,20,"Join Game");

  portLabel = UI::Label(ww/2+30,wh/2-80,20,"8080");

  joinSessLabel = UI::Label(ww/2-100, wh/2-50, 20, "Join Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  keyboard = UI::Keyboard(0,wh-100,ww,100);

  String s = Network::getIP();
  for(input_len = 0; *(s.ptr()+input_len) != '\0'; input_len++)
    input[input_len] = *(s.ptr()+input_len);

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
    if(!client->healthy()) client->connect(String(input,input_len),8080);
  }
}

int JoinScene::tick()
{
  if(client && client->healthy())
  {
    c_model = new ClientModel(client);
    *c_model_ptr = c_model;
    SCENE_CHANGE_HACK = 1;
  }
  else
  {
    char c = keyboard.poll();
    if(c == '<' && input_len > 0) input_len--;
    else if(c != 0 && c != '<') { input[input_len] = c; input_len++; }
  }

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void JoinScene::draw()
{
  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton.draw(graphics);
  joinGameLabel.draw(graphics);

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,&input[0],input_len);
  ipLabel.draw(graphics);

  portLabel.draw(graphics);

  joinSessLabel.draw(graphics);
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
  if(client) { if(client->healthy()) client->disconnect(); delete client; client = 0; *client_ptr = 0; }
}


JoinScene::~JoinScene()
{
}

