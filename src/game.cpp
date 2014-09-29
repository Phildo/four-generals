#include "game.h"
#include "graphics.h"
#include "network.h"
#include "model.h"
#include "sprite.h"
#include "logger.h"
#include <SDL.h>

const int FPS = 60;
const int MS_PER_TICK = 1000/FPS;

Game::Game()
{
  graphics = new Graphics();
  network = new Network();
  model = new Model();
}

void Game::run()
{
  fg_log("Begin!");
  Uint8 done = 0;
  SDL_Event event;

  float x, y;
  network->connectAsClient();
  while(!done)
  {
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        done = 1;
      }
      else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
      {
        x = (float)event.button.x/(float)graphics->winWidth();
        y = (float)event.button.y/(float)graphics->winHeight();
        fg_log("mouse");
      }
      else if(event.type == SDL_FINGERDOWN)
      {
        x = event.tfinger.x;
        y = event.tfinger.y;
        fg_log("finger");
        char c[] = "whatup";
        network->broadcast(c,6);
      }
    }

    graphics->clear();
    graphics->flip();
    SDL_Delay(10);
  }
}

Game::~Game()
{
  delete graphics;
  delete network;
  delete model;
}

