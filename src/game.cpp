#include "game.h"
#include "graphics.h"
#include "input.h"
#include "network.h"
#include "model.h"
#include "scene.h"

#include "intro_scene.h"
#include "host_scene.h"
#include "join_scene.h"
#include "room_scene.h"

#include "sprite.h"
#include "logger.h"

#include <SDL.h>

//const int FPS = 60;
//const int MS_PER_TICK = 1000/FPS;

Game::Game()
{
  graphics = new Graphics();
  input = new Input(graphics);
  //save allocation until necessary
  server = 0;
  client = 0;
  model = new Model();
  scenes[0] = new IntroScene(graphics);
  scenes[1] = new HostScene(graphics, server, client);
  scenes[2] = new JoinScene(graphics, client);
  scenes[3] = new RoomScene(graphics, model);
}

void Game::run()
{
  bool q = false;
  bool p = false;
  In in;
  int scene = 0;

  while(!q)
  {
    while(!q && input->poll(in, p, q))
      if(p) scenes[scene]->touch(in);

    scene += scenes[scene]->tick(); //should decouple from drawing

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
  if(client) delete client;
  if(server) delete server;
  delete input;
  delete graphics;
}

