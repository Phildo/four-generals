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
        Defend
          When
      Sabotage
        Block
        Read
        Switch
          What
            Attack
            Defend
          Who
          When
    */

    //choose_ (what)
    UI::ImageButtonRound choose_attack_image;   UI::Label choose_attack_label;
    UI::ImageButtonRound choose_defend_image;   UI::Label choose_defend_label;
    UI::ImageButtonRound choose_message_image;  UI::Label choose_message_label;
    UI::ImageButtonRound choose_sabotage_image; UI::Label choose_sabotage_label;

    //choose_attack_ (who)
    UI::ImageButtonRound choose_attack_attack_image; UI::Label choose_attack_attack_label;
    UI::ImageButtonRound choose_attack_who_image;    UI::Label choose_attack_who_label;
    UI::Button choose_attack_who_button_cw;
    UI::Button choose_attack_who_button_ccw;
    UI::TextButton choose_attack_cancel_button;

    //choose_attack_who (confirm)
    UI::ImageButtonRound choose_attack_who_attack_image; UI::Label choose_attack_who_attack_label;
    UI::ImageButtonRound choose_attack_who_who_image;    UI::Label choose_attack_who_who_label;
    UI::TextButton choose_attack_who_cancel_button;
    UI::TextButton choose_attack_who_confirm_button;

    //choose_defend (confirm)
    UI::ImageButtonRound choose_defend_defend_image; UI::Label choose_defend_defend_label;
    UI::TextButton choose_defend_cancel_button;
    UI::TextButton choose_defend_confirm_button;

    //choose_message_ (what)
    UI::ImageButtonRound choose_message_message_image; UI::Label choose_message_message_label;
    UI::ImageButtonRound choose_message_attack_image;  UI::Label choose_message_attack_label;
    UI::ImageButtonRound choose_message_defend_image;  UI::Label choose_message_defend_label;
    UI::TextButton choose_message_cancel_button;

    //choose_message_attack_ (who)
    UI::ImageButtonRound choose_message_attack_message_image; UI::Label choose_message_attack_message_label;
    UI::ImageButtonRound choose_message_attack_attack_image;  UI::Label choose_message_attack_attack_label;
    UI::ImageButtonRound choose_message_attack_who_image;     UI::Label choose_message_attack_who_label;
    UI::Button choose_message_attack_who_button_cw;
    UI::Button choose_message_attack_who_button_ccw;
    UI::TextButton choose_message_attack_cancel_button;

    //choose_message_attack_who_ (when)
    UI::ImageButtonRound choose_message_attack_who_message_image; UI::Label choose_message_attack_who_message_label;
    UI::ImageButtonRound choose_message_attack_who_attack_image;  UI::Label choose_message_attack_who_attack_label;
    UI::ImageButtonRound choose_message_attack_who_who_image;     UI::Label choose_message_attack_who_who_label;
    UI::ImageButtonRound choose_message_attack_who_when_image;    UI::Label choose_message_attack_who_when_label;
    UI::Button choose_message_attack_who_when_buttons[7];
    UI::TextButton choose_message_attack_who_cancel_button;

    //choose_message_attack_who_when_ (route)
    UI::ImageButtonRound choose_message_attack_who_when_message_image; UI::Label choose_message_attack_who_when_message_label;
    UI::ImageButtonRound choose_message_attack_who_when_attack_image;  UI::Label choose_message_attack_who_when_attack_label;
    UI::ImageButtonRound choose_message_attack_who_when_who_image;     UI::Label choose_message_attack_who_when_who_label;
    UI::ImageButtonRound choose_message_attack_who_when_when_image;    UI::Label choose_message_attack_who_when_when_label;
    UI::ImageButtonRound choose_message_attack_who_when_route_image;   UI::Label choose_message_attack_who_when_route_label;
    UI::Button choose_message_attack_who_when_route_button_cw;
    UI::Button choose_message_attack_who_when_route_button_ccw;
    UI::TextButton choose_message_attack_who_when_cancel_button;

    //choose_message_attack_who_when_route (confirm)
    UI::ImageButtonRound choose_message_attack_who_when_route_message_image; UI::Label choose_message_attack_who_when_route_message_label;
    UI::ImageButtonRound choose_message_attack_who_when_route_attack_image;  UI::Label choose_message_attack_who_when_route_attack_label;
    UI::ImageButtonRound choose_message_attack_who_when_route_who_image;     UI::Label choose_message_attack_who_when_route_who_label;
    UI::ImageButtonRound choose_message_attack_who_when_route_when_image;    UI::Label choose_message_attack_who_when_route_when_label;
    UI::ImageButtonRound choose_message_attack_who_when_route_route_image;   UI::Label choose_message_attack_who_when_route_route_label;
    UI::TextButton choose_message_attack_who_when_route_cancel_button;
    UI::TextButton choose_message_attack_who_when_route_confirm_button;

    //choose_message_defend_ (when)
    UI::ImageButtonRound choose_message_defend_message_image; UI::Label choose_message_defend_message_label;
    UI::ImageButtonRound choose_message_defend_defend_image;  UI::Label choose_message_defend_defend_label;
    UI::ImageButtonRound choose_message_defend_when_image;    UI::Label choose_message_defend_when_label;
    UI::Button choose_defend_when_buttons[7];
    UI::TextButton choose_message_defend_cancel_button;

    //choose_message_defend_when_ (route)
    UI::ImageButtonRound choose_message_defend_when_message_image; UI::Label choose_message_defend_when_message_label;
    UI::ImageButtonRound choose_message_defend_when_defend_image;  UI::Label choose_message_defend_when_defend_label;
    UI::ImageButtonRound choose_message_defend_when_when_image;    UI::Label choose_message_defend_when_when_label;
    UI::ImageButtonRound choose_message_defend_when_route_image;   UI::Label choose_message_defend_when_route_label;
    UI::Button choose_message_defend_when_route_button_cw;
    UI::Button choose_message_defend_when_route_button_ccw;
    UI::TextButton choose_message_defend_when_cancel_button;

    //choose_message_defend_when_route (confirm)
    UI::ImageButtonRound choose_message_defend_when_route_message_image; UI::Label choose_message_defend_when_route_message_label;
    UI::ImageButtonRound choose_message_defend_when_route_defend_image;  UI::Label choose_message_defend_when_route_defend_label;
    UI::ImageButtonRound choose_message_defend_when_route_when_image;    UI::Label choose_message_defend_when_route_when_label;
    UI::ImageButtonRound choose_message_defend_when_route_route_image;   UI::Label choose_message_defend_when_route_route_label;
    UI::TextButton choose_message_defend_when_route_cancel_button;
    UI::TextButton choose_message_defend_when_route_confirm_button;

    //choose_sabotage_ (how)
    UI::ImageButtonRound choose_sabotage_sabotage_image; UI::Label choose_sabotage_sabotage_label;
    UI::ImageButtonRound choose_sabotage_block_image;    UI::Label choose_sabotage_block_label;
    UI::ImageButtonRound choose_sabotage_read_image;     UI::Label choose_sabotage_read_label;
    UI::ImageButtonRound choose_sabotage_switch_image;   UI::Label choose_sabotage_switch_label;
    UI::TextButton choose_sabotage_cancel_button;

    //choose_sabotage_block (confirm)
    UI::ImageButtonRound choose_sabotage_block_sabotage_image; UI::Label choose_sabotage_block_sabotage_label;
    UI::ImageButtonRound choose_sabotage_block_block_image;    UI::Label choose_sabotage_block_block_label;
    UI::TextButton choose_sabotage_block_cancel_button;
    UI::TextButton choose_sabotage_block_confirm_button;

    //choose_sabotage_read (confirm)
    UI::ImageButtonRound choose_sabotage_read_sabotage_image; UI::Label choose_sabotage_read_sabotage_label;
    UI::ImageButtonRound choose_sabotage_read_read_image;     UI::Label choose_sabotage_read_read_label;
    UI::TextButton choose_sabotage_read_cancel_button;
    UI::TextButton choose_sabotage_read_confirm_button;

    //choose_sabotage_switch_ (what)
    UI::ImageButtonRound choose_sabotage_switch_sabotage_image; UI::Label choose_sabotage_switch_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_switch_image;   UI::Label choose_sabotage_switch_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_what_image;     UI::Label choose_sabotage_switch_what_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_image;      UI::Label choose_sabotage_switch_whot_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_image;     UI::Label choose_sabotage_switch_whent_label;
    UI::TextButton choose_sabotage_switch_cancel_button;

    //choose_sabotage_switch_what_ (what)
    UI::ImageButtonRound choose_sabotage_switch_what_sabotage_image; UI::Label choose_sabotage_switch_what_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_what_switch_image;   UI::Label choose_sabotage_switch_what_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_what_what_image;     UI::Label choose_sabotage_switch_what_what_label;
    UI::ImageButtonRound choose_sabotage_switch_what_attack_image;   UI::Label choose_sabotage_switch_what_attack_label;
    UI::ImageButtonRound choose_sabotage_switch_what_defend_image;   UI::Label choose_sabotage_switch_what_defend_label;
    UI::TextButton choose_sabotage_switch_what_cancel_button;

    //choose_sabotage_switch_what_attack (confirm)
    UI::ImageButtonRound choose_sabotage_switch_what_attack_sabotage_image; UI::Label choose_sabotage_switch_what_attack_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_what_attack_switch_image;   UI::Label choose_sabotage_switch_what_attack_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_what_attack_what_image;     UI::Label choose_sabotage_switch_what_attack_what_label;
    UI::ImageButtonRound choose_sabotage_switch_what_attack_attack_image;   UI::Label choose_sabotage_switch_what_attack_attack_label;
    UI::TextButton choose_sabotage_switch_what_attack_cancel_button;
    UI::TextButton choose_sabotage_switch_what_attack_confirm_button;

    //choose_sabotage_switch_what_defend (confirm)
    UI::ImageButtonRound choose_sabotage_switch_what_defend_sabotage_image; UI::Label choose_sabotage_switch_what_defend_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_what_defend_switch_image;   UI::Label choose_sabotage_switch_what_defend_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_what_defend_what_image;     UI::Label choose_sabotage_switch_what_defend_what_label;
    UI::ImageButtonRound choose_sabotage_switch_what_defend_defend_image;   UI::Label choose_sabotage_switch_what_defend_defend_label;
    UI::TextButton choose_sabotage_switch_what_defend_cancel_button;
    UI::TextButton choose_sabotage_switch_what_defend_confirm_button;

    //choose_sabotage_switch_whot_ (who)
    UI::ImageButtonRound choose_sabotage_switch_whot_sabotage_image; UI::Label choose_sabotage_switch_whot_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_switch_image;   UI::Label choose_sabotage_switch_whot_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_whot_image;      UI::Label choose_sabotage_switch_whot_whot_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_who_image;      UI::Label choose_sabotage_switch_whot_who_label;
    UI::Button choose_sabotage_switch_whot_who_button_cw;
    UI::Button choose_sabotage_switch_whot_who_button_ccw;
    UI::TextButton choose_sabotage_switch_whot_cancel_button;

    //choose_sabotage_switch_whot_who (confirm)
    UI::ImageButtonRound choose_sabotage_switch_whot_who_sabotage_image; UI::Label choose_sabotage_switch_whot_who_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_who_switch_image;   UI::Label choose_sabotage_switch_whot_who_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_who_whot_image;      UI::Label choose_sabotage_switch_whot_who_whot_label;
    UI::ImageButtonRound choose_sabotage_switch_whot_who_who_image;      UI::Label choose_sabotage_switch_whot_who_who_label;
    UI::TextButton choose_sabotage_switch_whot_who_cancel_button;
    UI::TextButton choose_sabotage_switch_whot_who_confirm_button;

    //choose_sabotage_switch_whent_ (when)
    UI::ImageButtonRound choose_sabotage_switch_whent_sabotage_image; UI::Label choose_sabotage_switch_whent_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_switch_image;   UI::Label choose_sabotage_switch_whent_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_whent_image;      UI::Label choose_sabotage_switch_whent_whent_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_when_image;      UI::Label choose_sabotage_switch_whent_when_label;
    UI::Button choose_sabotage_switch_whent_when_buttons[7];
    UI::TextButton choose_sabotage_switch_whent_cancel_button;

    //choose_sabotage_switch_whent_when (confirm)
    UI::ImageButtonRound choose_sabotage_switch_whent_when_sabotage_image; UI::Label choose_sabotage_switch_whent_when_sabotage_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_when_switch_image;   UI::Label choose_sabotage_switch_whent_when_switch_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_when_whent_image;      UI::Label choose_sabotage_switch_whent_when_whent_label;
    UI::ImageButtonRound choose_sabotage_switch_whent_when_when_image;      UI::Label choose_sabotage_switch_whent_when_when_label;
    UI::TextButton choose_sabotage_switch_whent_when_cancel_button;
    UI::TextButton choose_sabotage_switch_whent_when_confirm_button;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    char known_day;
    float anim_day;
    float shown_day;

    Event e; //populates with tentative event (holds state for what to show on screen)
    void zeroE();

    void chooseShownDay(In &in);

    void chooseWhat(In &in);
    void chooseAttackWho(In &in);
    void chooseAttackWhoConfirm(In &in);
    void chooseDefendConfirm(In &in);
    void chooseMessageWhat(In &in);
    void chooseMessageAttackWho(In &in);
    void chooseMessageAttackWhoWhen(In &in);
    void chooseMessageAttackWhoWhenRoute(In &in);
    void chooseMessageAttackWhoWhenRouteConfirm(In &in);
    void chooseMessageDefendWhen(In &in);
    void chooseMessageDefendWhenRoute(In &in);
    void chooseMessageDefendWhenRouteConfirm(In &in);
    void chooseSabotageHow(In &in);
    void chooseSabotageBlockConfirm(In &in);
    void chooseSabotageReadConfirm(In &in);
    void chooseSabotageSwitchWhat(In &in);
    void chooseSabotageSwitchWhatWhat(In &in);
    void chooseSabotageSwitchWhatAttackConfirm(In &in);
    void chooseSabotageSwitchWhatDefendConfirm(In &in);
    void chooseSabotageSwitchWhoWho(In &in);
    void chooseSabotageSwitchWhoWhoConfirm(In &in);
    void chooseSabotageSwitchWhenWhen(In &in);
    void chooseSabotageSwitchWhenWhenConfirm(In &in);

    void drawWhat();
    void drawAttackWho();
    void drawAttackWhoConfirm();
    void drawDefendConfirm();
    void drawMessageWhat();
    void drawMessageAttackWho();
    void drawMessageAttackWhoWhen();
    void drawMessageAttackWhoWhenRoute();
    void drawMessageAttackWhoWhenRouteConfirm();
    void drawMessageDefendWhen();
    void drawMessageDefendWhenRoute();
    void drawMessageDefendWhenRouteConfirm();
    void drawSabotageHow();
    void drawSabotageBlockConfirm();
    void drawSabotageReadConfirm();
    void drawSabotageSwitchWhat();
    void drawSabotageSwitchWhatWhat();
    void drawSabotageSwitchWhatAttackConfirm();
    void drawSabotageSwitchWhatDefendConfirm();
    void drawSabotageSwitchWhoWho();
    void drawSabotageSwitchWhoWhoConfirm();
    void drawSabotageSwitchWhenWhen();
    void drawSabotageSwitchWhenWhenConfirm();

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

