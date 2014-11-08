#ifndef _FG_ROOM_SCENE_H_
#define _FG_ROOM_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;
class ServerModel;
class ClientModel;

class RoomScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label inRoomLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Label nLabel; UI::Button nButton; UI::Box nBox;
    UI::Label eLabel; UI::Button eButton; UI::Box eBox;
    UI::Label sLabel; UI::Button sButton; UI::Box sBox;
    UI::Label wLabel; UI::Button wButton; UI::Box wBox;

    UI::Label leaveSessLabel;
    UI::Button leaveSessButton;

    UI::Label beginGameLabel;
    UI::Button beginGameButton;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    int SCENE_CHANGE_HACK;
  public:
    RoomScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm);
    ~RoomScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

