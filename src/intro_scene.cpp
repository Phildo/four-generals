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

  hostLabel  = UI::Label( g->winWidth()-225,25,    20,"Host",4);
  hostButton = UI::Button(g->winWidth()-225,25,200,20);

  joinLabel  = UI::Label( g->winWidth()-225,55,    20,"Join",4);
  joinButton = UI::Button(g->winWidth()-225,55,200,20);

  howLabel  = UI::Label( g->winWidth()-225,85,    20,"How",4);
  howButton = UI::Button(g->winWidth()-225,85,200,20);
}

void IntroScene::touch(In &in)
{
  if(hostButton.query(in)) fg_log("touched s!");
  if(joinButton.query(in)) fg_log("touched j!");
  if(howButton.query(in)) fg_log("touched h!");
}

void IntroScene::tick()
{
}

void IntroScene::draw()
{
  hostLabel.draw(graphics);
  hostButton.draw(graphics);

  joinLabel.draw(graphics);
  joinButton.draw(graphics);

  howLabel.draw(graphics);
  howButton.draw(graphics);
}

Scene::~Scene()
{

}

