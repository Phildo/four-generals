#include "intro_scene.h"
#include "logger.h"
#include "sprite.h"

IntroScene::IntroScene(Graphics *g)
{
  graphics = g;
}

void IntroScene::touch(In &in)
{
  fg_log("touched %d, %d",in.x,in.y);
  char c[] = "whatup\n";
  //network->broadcast(c,7);

  fg_log("width:%d height:%d",graphics->winWidth(),graphics->winHeight());
}

void IntroScene::tick()
{

}

void IntroScene::draw()
{
  graphics->drawAt(Sprite::tl_border(),20,20);
  graphics->drawAt(Sprite::t_border(), 30,20);
  graphics->drawAt(Sprite::t_border(), 40,20);
  graphics->drawAt(Sprite::t_border(), 50,20);
  graphics->drawAt(Sprite::t_border(), 60,20);
  graphics->drawAt(Sprite::t_border(), 70,20);
  graphics->drawAt(Sprite::tr_border(),80,20);
  graphics->drawAt(Sprite::r_border(), 80,30);
  graphics->drawAt(Sprite::r_border(), 80,40);
  graphics->drawAt(Sprite::br_border(),80,50);
  graphics->drawAt(Sprite::b_border(), 70,50);
  graphics->drawAt(Sprite::b_border(), 60,50);
  graphics->drawAt(Sprite::b_border(), 50,50);
  graphics->drawAt(Sprite::b_border(), 40,50);
  graphics->drawAt(Sprite::b_border(), 30,50);
  graphics->drawAt(Sprite::bl_border(),20,50);
  graphics->drawAt(Sprite::l_border(), 20,40);
  graphics->drawAt(Sprite::l_border(), 20,30);
}

Scene::~Scene()
{

}

