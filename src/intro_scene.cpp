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
  b = UI::Button(20,20,100,50);
}

void IntroScene::touch(In &in)
{
  if(b.query(in)) fg_log("touched!");
  else fg_log("nope");
}

void IntroScene::tick()
{

}

void IntroScene::draw()
{
  b.draw(graphics);
}

Scene::~Scene()
{

}

