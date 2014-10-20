#ifndef _FG_ROOM_SCENE_H_
#define _FG_ROOM_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;
class Model;

class RoomScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label inRoomLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Label nLabel; UI::Label npLabel; UI::Button nButton;
    UI::Label sLabel; UI::Label spLabel; UI::Button sButton;
    UI::Label wLabel; UI::Label wpLabel; UI::Button wButton;
    UI::Label eLabel; UI::Label epLabel; UI::Button eButton;

    UI::Label leaveSessLabel;
    UI::Button sessionButton;

    Network::Client *client;
    Network::Client **clientPtr;//Pointer to the client pointer 'owned' by game
    Model *model;
    Model **modelPtr;//Pointer to the model pointer 'owned' by game

  public:
    RoomScene(Graphics *g, Network::Client *& c, Model *&m);
    RoomScene(Graphics *g, Model *m);
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

