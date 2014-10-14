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
  public:
    ClientScene(Graphics *g, Network::Client *& c);
    ~ClientScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

