#include "intro_scene.h"
#include "logger.h"

IntroScene::IntroScene(Graphics *g)
{
  graphics = g;
}

void IntroScene::touch(In &in)
{
  fg_log("touched");
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

