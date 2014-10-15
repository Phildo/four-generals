#ifndef _FG_HOST_SCENE_H_
#define _FG_HOST_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;

class HostScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label hostGameLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Label startSessLabel;
    UI::Button sessionButton;

    Network::Server *server;
    Network::Client *client;

    //Pointers to the server and client pointers 'owned' by game
    Network::Server **serverPtr;
    Network::Client **clientPtr;

    int SCENE_CHANGE_HACK;
  public:
    HostScene(Graphics *g, Network::Server *& s, Network::Client *& c);
    ~HostScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

