#ifndef _FG_ROOM_SCENE_H_
#define _FG_ROOM_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;

class RoomScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label hostGameLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Label nLabel; UI::Label npLabel; UI::Button nButton;
    UI::Label sLabel; UI::Label spLabel; UI::Button sButton;
    UI::Label wLabel; UI::Label wpLabel; UI::Button wButton;
    UI::Label eLabel; UI::Label epLabel; UI::Button eButton;

    UI::Label startSessLabel;
    UI::Label cancelSessLabel;
    UI::Label joinSessLabel;
    UI::Label leaveSessLabel;
    UI::Button sessionButton;

    UI::Label inRoomLabel;

    Model *model;
  public:
    RoomScene(Graphics *g, Model *m);
    ~RoomScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif
