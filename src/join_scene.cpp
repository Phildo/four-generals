#include "join_scene.h"
#include "graphics.h"
#include "network.h"

#include <SDL.h>

#include "logger.h"

JoinScene::JoinScene(Graphics *g, Network::Client *& c)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(10,10,20,20);

  joinGameLabel = UI::Label(ww/2-100,wh/2-100,20,"Join Game");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+30,wh/2-80,20,"8080");

  joinSessLabel = UI::Label(ww/2-100, wh/2-50, 20, "Join Session");
  sessionButton  = UI::Button(ww/2-100, wh/2-50, 200, 20);

  clientPtr = &c;
  client = 0;

  SCENE_CHANGE_HACK = 0;
}

void JoinScene::touch(In &in)
{
  if(backButton.query(in))
  {
    fg_log("backButton");
    SCENE_CHANGE_HACK = -2;
  }
  if(sessionButton.query(in))
  {
    fg_log("sessionButton");
    if(!client) { client = new Network::Client(); *clientPtr = client; }
    if(!client->healthy()) client->connect(String("localhost"),8080);
  }
}

int JoinScene::tick()
{
  if(client && client->healthy())
    SCENE_CHANGE_HACK = 1;

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void JoinScene::draw()
{
  backButton.draw(graphics);
  joinGameLabel.draw(graphics);
  ipLabel.draw(graphics);
  portLabel.draw(graphics);

  joinSessLabel.draw(graphics);
  sessionButton.draw(graphics);
}

JoinScene::~JoinScene()
{
}

