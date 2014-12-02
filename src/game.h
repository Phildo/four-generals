#ifndef _FG_GAME_H_
#define _FG_GAME_H_

#include "ui.h"

class Graphics;
class Input;
namespace Network
{
class Server;
class Client;
}
class ServerModel;
class ClientModel;
class Scene;

#define FG_NUM_SCENES 5

class Game
{
  private:
    Graphics* graphics;
    Input *input;
    Network::Server* server;
    Network::Client* client;
    ServerModel* s_model;
    ClientModel* c_model;
    Scene* scenes[FG_NUM_SCENES];

    bool debug_toggle;
    UI::Button debugBtn;
    UI::Box border;
  public:
    Game();
    ~Game();

    void run();
};

#endif

