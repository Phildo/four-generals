#ifndef _GAME_H_
#define _GAME_H_

#include "var_array.h"
#include "scene.h"

class Graphics;
class Network;
class Model;

class Game
{
  private:
    Graphics* graphics;
    Network* network;
    Model* model;
    Scene** scenes;

  public:
    Game();
    ~Game();

    void run();
};

#endif

