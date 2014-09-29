#include "intro_scene.h"
#include "logger.h"

IntroScene::IntroScene(Graphics *g, Network *n)
{
  network = n;
  graphics = g;
}

void IntroScene::touch(In &in)
{
  fg_log("touched %d, %d",in.x,in.y);
  char c[] = "whatup\n";
  network->broadcast(c,7);
}

void IntroScene::tick()
{

}

void IntroScene::draw()
{

}

Scene::~Scene()
{

}

