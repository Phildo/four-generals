#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "lerp_rect.h"
#include "network.h"
#include "event.h"

class Graphics;
class ServerModel;
class ClientModel;

class PlayScene : public Scene
{
  private:
    Graphics *graphics;

    //sprites
    UI::AnimSprites generals_s[4];
    SDL_Rect pTags[4];
    SDL_Rect pTagsW[4];
    SDL_Rect pTagsB[4];
    SDL_Rect pTagsR[4];
    SDL_Rect force_field_s;
    SDL_Rect shield_full_s;
    SDL_Rect shield_broken_s;
    SDL_Rect sword_s;
    SDL_Rect red_x_s;
    SDL_Rect envelope_s;
    SDL_Rect sun_s;
    SDL_Rect sblock_s;
    SDL_Rect sread_s;
    SDL_Rect sswitch_s;
    SDL_Rect what_s;
    SDL_Rect who_ns_s;
    SDL_Rect who_we_s;
    SDL_Rect when_s;
    SDL_Rect where_ns_s;
    SDL_Rect where_we_s;
    SDL_Rect null_s;

    //rects
    SDL_Rect positionRects[4];
    SDL_Rect dayRects[7];
    SDL_Rect sunRects[7];

    UI::Label dayLbls[7];
    bool sunDragging;
    UI::Button sunBtn;

    bool sabotage_0_reading;
    UI::ImageButtonRound read_sabotage_0;
    bool sabotage_1_reading;
    UI::ImageButtonRound read_sabotage_1;
    bool message_reading;
    UI::ImageButtonRound read_message;

    UI::Anim cardImgs[4];
    UI::Label cardLbls[4];

    /*
    Choose
      Attack
        Who
      Defend
      Message
        Attack
          Who
            When
      Sabotage
        Block
        Read
        Switch
          Who
          When
    */

    UI::Anim loading;
    UI::Label waiting_on_players_label;
    UI::TextButton reset_game_button;
    UI::Image win_img;
    UI::Image lose_img;
    UI::Image tie_img;

    void drawWaiting();
    void drawReset();
    void drawWin();
    void drawLose();
    void drawTie();

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    char known_day;
    float anim_day;
    float shown_day;

    Turn t; //populates with tentative turn (holds state for what to show on screen)

    void chooseShownDay(In &in);
    void chooseReadSabotage(In &in);
    void chooseReadMessage(In &in);

    void drawSabotage0();
    void drawSabotage1();
    void drawMessage();

    SDL_Rect rectForPosition(char c);
    SDL_Rect rectForCardinal(char c);
    SDL_Rect rectForDay(char d);
    SDL_Rect rectForSun(char d);
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

