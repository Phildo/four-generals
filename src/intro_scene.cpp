#include "intro_scene.h"
#include "graphics.h"
#include "input.h"
#include "ui.h"
#include "network.h"
#include "sprite.h"

#include <SDL.h>

#include "logger.h"

IntroScene::IntroScene(Graphics *g)
{
  graphics = g;
  int l = Network::getIP(Network::ip);
  ipString = UI::String(25,25,Network::ip,l);
  connectServerButton = UI::Button(20,60,100,50);
  connectClientButton = UI::Button(80,60,100,50);
}

void IntroScene::touch(In &in)
{
  if(connectServerButton.query(in)) fg_log("touched s!");
  if(connectClientButton.query(in)) fg_log("touched c!");
  else fg_log("nope");
}

void IntroScene::tick()
{

}

void IntroScene::draw()
{
  ipString.draw(graphics);
  connectServerButton.draw(graphics);
  connectClientButton.draw(graphics);
}

Scene::~Scene()
{

}

