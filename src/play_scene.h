#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;
class ServerModel;
class ClientModel;

class PlayScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label sunLabel;
    UI::Label monLabel;
    UI::Label tueLabel;
    UI::Label wedLabel;
    UI::Label thuLabel;
    UI::Label friLabel;
    UI::Label satLabel;

    UI::Label nLabel; UI::Button nButton;
    UI::Label sLabel; UI::Button sButton;
    UI::Label wLabel; UI::Button wButton;
    UI::Label eLabel; UI::Button eButton;

    UI::Label attackLabel; UI::Button attackButton;
    UI::Label defendLabel; UI::Button defendButton;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s_model;
    ServerModel **s_model_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c_model;
    ClientModel **c_model_ptr; //Pointers to the client model pointer 'owned' by game

  public:
    PlayScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm);
    ~PlayScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

