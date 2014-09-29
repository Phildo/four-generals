#include "intro_scene.h"
#include "logger.h"
#include "sprite.h"

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

  fg_log("width:%d height:%d",graphics->winWidth(),graphics->winHeight());
}

void IntroScene::tick()
{

}

void IntroScene::draw()
{
  graphics->drawAt(Sprite::tl_border(),0,0);
}

Scene::~Scene()
{

}

