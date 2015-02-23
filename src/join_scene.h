#ifndef _FG_JOIN_SCENE_H_
#define _FG_JOIN_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"
#include "string.h"

class Graphics;
class ServerModel;
class ClientModel;

class JoinScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label joinGameLabel;

    UI::TextButton manual;
    UI::TextButton automatic;
    bool manualEntry;

    UI::TextButton searchJoinButton;
    String ipPrefix;
    int searching;

    UI::TextButton joinButton;
    UI::Label portLabel;
    UI::TextBox ipInput;
    UI::Keyboard keyboard;

    UI::Anim loading;

    Network::Client *client;
    Network::Client **client_ptr; //Pointer to the client pointer 'owned' by game
    ClientModel *c_model;
    ClientModel **c_model_ptr; //Pointer to the client model pointer 'owned' by game

    int SCENE_CHANGE_HACK;
    void reset();
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

