#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "lerp_rect.h"
#include "network.h"

#include "turn_picker.h"

class Graphics;
class ServerModel;
class ClientModel;

class PlayScene : public Scene
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
    SDL_Rect cardRects[4];
    SDL_Rect cardLabelRects[4];
    SDL_Rect dayRects[7];
    SDL_Rect sunRects[7];

    //sprites
    UI::AnimSprites generals_s[4];
    SDL_Rect pTags[4];
    SDL_Rect pTagsW[4];
    SDL_Rect pTagsB[4];
    SDL_Rect pTagsR[4];

    //views
    UI::Anim cardImgs[4];
    UI::Label cardLbls[4];

    UI::Label dayLbls[7];
    UI::Button sunBtn;

    UI::ImageButtonRound read_sabotage_0;
    UI::ImageButtonRound read_sabotage_1;
    UI::ImageButtonRound read_message;

    UI::Anim loading;
    UI::Label waiting_on_players_label;
    UI::TextButton reset_game_button;
    UI::Image win_img;
    UI::Image lose_img;
    UI::Image tie_img;

    TurnPicker picker;

    //state
    float goal_days;
    float current_days;
    float shown_days;

    bool sunDragging;
    bool sabotage_0_reading;
    bool sabotage_1_reading;
    bool message_reading;

    void drawWaiting();
    void drawReset();
    void drawWin();
    void drawLose();
    void drawTie();

    void chooseShownDay(In &in);

    SDL_Rect rectForTraversal(char fcard, char tcard, float t);
    SDL_Rect rectForExpansion(char card, float t);
    SDL_Rect rectForTransition(char fd, char td, float t);

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

