#include "intro_scene.h"
#include "graphics.h"
#include "input.h"
#include "ui.h"
#include "network.h"
#include "sprite.h"

#include <SDL.h>

#include "logger.h"

IntroScene::IntroScene(Graphics *g) : keyboard(0,100,300,100)
{
  graphics = g;
  int l = Network::getIP(Network::ip);
  ipString = UI::String(25,25,25,Network::ip,l);
  connectServerButton = UI::Button(20,60,100,50);
  connectServerButton = UI::Button(0,0,100,100);
  connectClientButton = UI::Button(80,60,100,50);
}

void IntroScene::touch(In &in)
{
  if(connectServerButton.query(in)) fg_log("touched s!");
  if(connectClientButton.query(in)) fg_log("touched c!");
  keyboard.touch(in);
}

void IntroScene::tick()
{
  char c;
  while((c = keyboard.poll()))
  {
    fg_log("touched %c",c);
  }
}

void IntroScene::draw()
{
  ipString.draw(graphics);
  connectServerButton.draw(graphics);
  connectClientButton.draw(graphics);
  keyboard.draw(graphics);
}

Scene::~Scene()
{

}

