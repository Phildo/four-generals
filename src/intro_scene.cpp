#include "intro_scene.h"
#include "graphics.h"

#include <SDL.h>

#include "logger.h"

IntroScene::IntroScene(Graphics *g)
{
  graphics = g;

  hostLabel  = UI::Label( g->winWidth()-225,25,    20,"Host",5);
  hostButton = UI::Button(g->winWidth()-225,25,200,20);

  joinLabel  = UI::Label( g->winWidth()-225,55,    20,"Join",5);
  joinButton = UI::Button(g->winWidth()-225,55,200,20);

  howLabel  = UI::Label( g->winWidth()-225,85,    20,"How to Play",12);
  howButton = UI::Button(g->winWidth()-225,85,200,20);

  SCENE_CHANGE_HACK = 0;
}

void IntroScene::enter()
{

}

void IntroScene::touch(In &in)
{
  if(hostButton.query(in)) { fg_log("touched s!"); SCENE_CHANGE_HACK = 1; }
  if(joinButton.query(in)) { fg_log("touched j!"); SCENE_CHANGE_HACK = 2; }
  if(howButton.query(in)) fg_log("touched h!");
}

int IntroScene::tick()
{
  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
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

void IntroScene::leave()
{

}
void IntroScene::pass()
{

}
void IntroScene::pop()
{

}

IntroScene::~IntroScene()
{

}

