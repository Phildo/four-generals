#include "game.h"
#include "graphics.h"
#include "ui.h"
#include "input.h"
#include "network.h"
#include "model.h"
#include "scene.h"

#include "intro_scene.h"

#include "sprite.h"
#include "logger.h"

#include <SDL.h>

const int FPS = 60;
const int MS_PER_TICK = 1000/FPS;

Game::Game()
{
  graphics = new Graphics();
  ui = new UI(graphics);
  input = new Input(graphics);
  network = new Network();
  model = new Model();
  scenes[0] = new IntroScene(graphics);
}

void Game::run()
{
  bool q = false;
  In in;
  int scene = 0;

  network->connectAsServer();
  while(!q)
  {
    while(input->poll(in, q))
      if(!q) scenes[scene]->touch(in);

    /*
    char c[] = "whatup\n";
    network->broadcast(c,7);
    */

    scenes[scene]->tick(); //should decouple from drawing
    graphics->clear();
    scenes[scene]->draw();
    graphics->flip();

    SDL_Delay(10);
  }
}

Game::~Game()
{
  for(int i = 0; i < FG_NUM_SCENES; i++)
    delete scenes[i];
  delete model;
  delete network;
  delete input;
  delete ui;
  delete graphics;
}

