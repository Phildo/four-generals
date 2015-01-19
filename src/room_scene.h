#ifndef _FG_ROOM_SCENE_H_
#define _FG_ROOM_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "lerp_rect.h"
#include "network.h"

class Graphics;
class ServerModel;
class ClientModel;

class RoomScene : public Scene
{
  private:
    Graphics *graphics;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    //rects
    SDL_Rect posRects[4];
    SDL_Rect posLabelRects[4];
    LerpRect cardRects[4];
    LerpRect cardLabelRects[4];

    //sprites
    UI::AnimSprites generals_s[4];
    SDL_Rect pTags[4];
    SDL_Rect pTagsW[4];
    SDL_Rect pTagsB[4];
    SDL_Rect pTagsR[4];

    //views
    UI::Anim cardImgs[4];
    UI::Label cardLbls[4];

    UI::Button backButton;

    UI::Label youAreLabel;
    UI::Image youPTag;
    UI::Image showIpQ;
    UI::Label netroleLabel;

    UI::Button showIpButton;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Image pScoreLabels[4];
    UI::Label pWinLabels[4];
    UI::Label pLossLabels[4];
    UI::Label pTieLabels[4];

    UI::TextButton beginGameButton;
    UI::Button chooseBtn;
    UI::ImageButton cwBtn;
    UI::ImageButton ccwBtn;

    //state
    int cardCWOffset; //current rotation
    bool showingIp;

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

