#ifndef _FG_GAME_H_
#define _FG_GAME_H_

#include "var_array.h"

class Graphics;
class Input;
namespace Network
{
class Server;
class Client;
}
class Model;
class Scene;

#define FG_NUM_SCENES 4

class Game
{
  private:
    Graphics* graphics;
    Input *input;
    Network::Server* server;
    Network::Client* client;
    Model* model;
    Scene* scenes[FG_NUM_SCENES];

  public:
    Game();
    ~Game();

    void run();
};

#endif

