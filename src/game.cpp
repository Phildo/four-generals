#include "game.h"
#include "graphics.h"
#include "input.h"
#include "server.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"
#include "scene.h"

#include "debug_list.h"

#include "intro_scene.h"
#include "host_scene.h"
#include "join_scene.h"
#include "room_scene.h"
#include "play_scene.h"

#include "sprite.h"
#include "logger.h"

#include <SDL.h>

//const int FPS = 60;
//const int MS_PER_TICK = 1000/FPS;

Game::Game()
{
  graphics = new Graphics();
  input = new Input(graphics);

  DebugList::inst()->init(graphics);

  //save allocation until necessary
  server = 0;
  client = 0;
  s_model = 0;
  c_model = 0;
  scenes[0] = new IntroScene(graphics);
  scenes[1] = new HostScene(graphics, server, client, s_model, c_model);
  scenes[2] = new JoinScene(graphics, client, c_model);
  scenes[3] = new RoomScene(graphics, client, s_model, c_model);
  scenes[4] = new PlayScene(graphics, client, s_model, c_model);

  debug_toggle = false;
  debugBtn = UI::Button(graphics->winWidth()-40,graphics->winHeight()-40,40,40);
  border = UI::Box(0,0,graphics->winWidth(),graphics->winHeight());
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
    {
      if(p)
      {
        scenes[scene]->touch(in);
        if(debugBtn.query(in)) debug_toggle = !debug_toggle;
      }
    }

    int tmp = scenes[scene]->tick(); //should decouple from drawing
    DebugList::inst()->tick();
    if(tmp != 0) scenes[scene]->leave();
    if(tmp >  0) scenes[scene]->pass();
    if(tmp <  0) scenes[scene]->pop();
    scene += tmp;
    if(tmp != 0) scenes[scene]->enter();

    graphics->clear();
    scenes[scene]->draw();
    debugBtn.draw(graphics);
    border.draw(graphics);
    if(debug_toggle) DebugList::inst()->draw(graphics);
    graphics->flip();

    SDL_Delay(30);
  }
}

Game::~Game()
{
  for(int i = FG_NUM_SCENES-1; i >= 0; i--)
    delete scenes[i];
  if(s_model) delete s_model;
  if(c_model) delete c_model;
  if(client) delete client;
  if(server) delete server;
  delete input;
  delete graphics;
}

