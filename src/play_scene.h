#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "lerp_rect.h"
#include "network.h"

#include "day_picker.h"
#include "turn_picker.h"
#include "messager.h"

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
    SDL_Rect posHealthLabelRects[4];
    SDL_Rect cardRects[4];
    SDL_Rect cardLabelRects[4];
    SDL_Rect cardHealthLabelRects[4];

    //views
    UI::Anim cardImgs[4];
    UI::Label cardLbls[4];
    UI::Label cardHealthLbls[4];

    UI::ImageButtonRound read_sabotage_0;
    UI::ImageButtonRound read_sabotage_1;
    UI::ImageButtonRound read_message;

    UI::Anim loading;
    UI::Label waiting_on_players_label;
    UI::TextButton reset_game_button;
    UI::Image win_img;
    UI::Image lose_img;
    UI::Image tie_img;

    DayPicker dayPicker;
    TurnPicker turnPicker;
    Messager messager;

    //state

    //note - all states can be interrupted by history viewing
    enum PLAY_SCENE_STATE //the visual state of the view
    {
      IDLE,         // can choose to look at messages, pick turn, or view history
      MESSAGE,      // full screen message/sabotage showing
      TURN_PICKING, // full screen action picker
      VIEWING,      // viewing history
      SHOWING,      // animating newly played out action (interaction disabled)
      COUNT
    } state;
    void setViewState(PLAY_SCENE_STATE s); //clears state vars and sets state

    float known_days;   //day known about in model
    float shown_days;   //day shown up til (animates toward goal)
    float showing_days; //currently showing day (animation, history)

    void drawWin();
    void drawLose();
    void drawTie();

    SDL_Rect rectForTraversal(char fcard, char tcard, float t);
    SDL_Rect rectForExpansion(char card, float t);

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

