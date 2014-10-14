#ifndef _FG_CLIENT_SCENE_H_
#define _FG_CLIENT_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;

class ClientScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label joinGameLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Label joinSessLabel;
    UI::Button sessionButton;

    UI::Label inRoomLabel;

    Network::Client *client;

    Model *model;
  public:
    ClientScene(Graphics *g, Model *m);
    ~ClientScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

