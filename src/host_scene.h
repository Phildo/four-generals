#ifndef _FG_HOST_SCENE_H_
#define _FG_HOST_SCENE_H_

#include "scene.h"
#include "network.h"

class Graphics;
class ServerModel;
class ClientModel;

class HostScene : public Scene
{
  private:
    Network::Server *server;
    Network::Server **serverPtr; //Pointers to the server pointer 'owned' by game
    Network::Client *client;
    Network::Client **clientPtr; //Pointers to the client pointer 'owned' by game
    ServerModel *s_model;
    ServerModel **s_model_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c_model;
    ClientModel **c_model_ptr; //Pointers to the client model pointer 'owned' by game

    int SCENE_CHANGE_HACK;
  public:
    HostScene(Graphics *g, Network::Server *& s, Network::Client *& c, ServerModel *& sm, ClientModel *& cm);
    ~HostScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

