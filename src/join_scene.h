#ifndef _FG_JOIN_SCENE_H_
#define _FG_JOIN_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;
class ServerModel;
class ClientModel;

class JoinScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label joinGameLabel;
    UI::Label portLabel;

    UI::Label joinSessLabel;
    UI::Button sessionButton;

    UI::TextBox ipInput;
    UI::Keyboard keyboard;

    Network::Client *client;
    Network::Client **client_ptr; //Pointer to the client pointer 'owned' by game
    ClientModel *c_model;
    ClientModel **c_model_ptr; //Pointer to the client model pointer 'owned' by game

    int SCENE_CHANGE_HACK;
  public:
    JoinScene(Graphics *g, Network::Client *& c, ClientModel *& cm);
    ~JoinScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

