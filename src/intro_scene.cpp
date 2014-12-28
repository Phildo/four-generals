#include "intro_scene.h"
#include "graphics.h"
#include "input.h"

#include "logger.h"

#include "SDL.h"

IntroScene::IntroScene(Graphics *g)
{
  graphics = g;

  hostButton = UI::TextButton("Host",g->winWidth()-450,50,400,40);
  joinButton = UI::TextButton("Join",g->winWidth()-450,110,400,40);
  howButton = UI::TextButton("How to Play",g->winWidth()-450,170,400,40);

  SCENE_CHANGE_HACK = 0;
}

void IntroScene::enter()
{

}

void IntroScene::touch(In &in)
{
  if(in.type != In::DOWN) return;
  if(hostButton.query(in)) { SCENE_CHANGE_HACK = 1; }
  if(joinButton.query(in)) { SCENE_CHANGE_HACK = 2; }
  if(howButton.query(in))  { SCENE_CHANGE_HACK = 0; }
}

int IntroScene::tick()
{
  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void IntroScene::draw()
{
  hostButton.draw(graphics);
  joinButton.draw(graphics);
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

