#include "play_scene.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"
#include "week.h"

#include "SDL.h"

PlayScene::PlayScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  client_ptr = &c;
  s_ptr = &sm;
  c_ptr = &cm;
  client = 0;
  s = 0;
  c = 0;

  generals_s[0] = UI::AnimSprites(Sprite::gen_n_0(), Sprite::gen_n_1(), Sprite::gen_n_2(), Sprite::gen_n_3());
  generals_s[1] = UI::AnimSprites(Sprite::gen_e_0(), Sprite::gen_e_1(), Sprite::gen_e_2(), Sprite::gen_e_3());
  generals_s[2] = UI::AnimSprites(Sprite::gen_s_0(), Sprite::gen_s_1(), Sprite::gen_s_2(), Sprite::gen_s_3());
  generals_s[3] = UI::AnimSprites(Sprite::gen_w_0(), Sprite::gen_w_1(), Sprite::gen_w_2(), Sprite::gen_w_3());

  pTags[0] = Sprite::p1();
  pTags[1] = Sprite::p2();
  pTags[2] = Sprite::p3();
  pTags[3] = Sprite::p4();
  pTagsW[0] = Sprite::p1_w();
  pTagsW[1] = Sprite::p2_w();
  pTagsW[2] = Sprite::p3_w();
  pTagsW[3] = Sprite::p4_w();
  pTagsB[0] = Sprite::p1_b();
  pTagsB[1] = Sprite::p2_b();
  pTagsB[2] = Sprite::p3_b();
  pTagsB[3] = Sprite::p4_b();
  pTagsR[0] = Sprite::p1_r();
  pTagsR[1] = Sprite::p2_r();
  pTagsR[2] = Sprite::p3_r();
  pTagsR[3] = Sprite::p4_r();

  force_field_s   = Sprite::shield_full_force();
  shield_full_s   = Sprite::shield_full();
  shield_broken_s = Sprite::shield_cracked();
  sword_s         = Sprite::sword();
  red_x_s         = Sprite::red_x();
  envelope_s      = Sprite::envelope();
  sun_s           = Sprite::sun();
  sblock_s        = Sprite::sblock();
  sread_s         = Sprite::sread();
  sswitch_s       = Sprite::sswitch();
  what_s          = Sprite::what();
  who_ns_s        = Sprite::who_ns();
  who_we_s        = Sprite::who_we();
  when_s          = Sprite::when();
  where_ns_s      = Sprite::where_ns();
  where_we_s      = Sprite::where_we();
  null_s          = Sprite::null();

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  int posRectW[] = {                90,                110,                120,                110};
  int posRectH[] = {                90,                110,                120,                110};
  int posRectX[] = {ww/2-posRectW[0]/2,  ww-posRectW[1]-20, ww/2-posRectW[2]/2,                 20};
  int posRectY[] = {                60, wh/2-posRectH[1]/2,  wh-posRectH[2]-40, wh/2-posRectH[3]/2};

  for(int i = 0; i < 4; i++)
  {
    positionRects[i].x = posRectX[i];
    positionRects[i].y = posRectY[i];
    positionRects[i].w = posRectW[i];
    positionRects[i].h = posRectH[i];
  }

  for(int i = 0; i < 7; i++)
  {
    dayRects[i].x = space(ww,60,40,7,i);
    dayRects[i].y = 20;
    dayRects[i].w = 40;
    dayRects[i].h = 40;

    sunRects[i].x = space(ww,50,60,7,i);
    sunRects[i].y = 10;
    sunRects[i].w = 60;
    sunRects[i].h = 60;
  }

  dayLbls[0] = UI::Label("Su", dayRects[0]);
  dayLbls[1] = UI::Label("Mo", dayRects[1]);
  dayLbls[2] = UI::Label("Tu", dayRects[2]);
  dayLbls[3] = UI::Label("We", dayRects[3]);
  dayLbls[4] = UI::Label("Th", dayRects[4]);
  dayLbls[5] = UI::Label("Fr", dayRects[5]);
  dayLbls[6] = UI::Label("Sa", dayRects[6]);

  sunBtn = UI::Button(dayRects[0]);
  sunDragging = false;

  sabotage_0_reading = false;
  read_sabotage_0 = UI::ImageButtonRound(   red_x_s,   10,wh-50,20,20);
  sabotage_1_reading = false;
  read_sabotage_1 = UI::ImageButtonRound(   red_x_s,   10,wh-80,20,20);
  message_reading = false;
  read_message    = UI::ImageButtonRound(envelope_s,ww-30,wh-50,20,20);

  cardImgs[Compass::icardinal('n')] = UI::Anim(generals_s[Compass::icardinal('n')], 4, 1.f, rectForPosition('n'));
  cardImgs[Compass::icardinal('e')] = UI::Anim(generals_s[Compass::icardinal('e')], 4, 1.f, rectForPosition('n'));
  cardImgs[Compass::icardinal('s')] = UI::Anim(generals_s[Compass::icardinal('s')], 4, 1.f, rectForPosition('n'));
  cardImgs[Compass::icardinal('w')] = UI::Anim(generals_s[Compass::icardinal('w')], 4, 1.f, rectForPosition('n'));

  cardLbls[Compass::icardinal('n')] = UI::Label("north", rectForPosition('n'));
  cardLbls[Compass::icardinal('e')] = UI::Label("east",  rectForPosition('n'));
  cardLbls[Compass::icardinal('s')] = UI::Label("south", rectForPosition('n'));
  cardLbls[Compass::icardinal('w')] = UI::Label("west",  rectForPosition('n'));


  //Templates for all positions (unused commented out to prevent warnings)

  int big_s = 100;
  int big_y = 220;
  int small_s = 50;
  int small_y = 250;

  int small_01_x = space(ww,200,small_s,1,0);

  int small_02_x = space(ww,200,small_s,2,0);
  int small_12_x = space(ww,200,small_s,2,1);
  //int small_x2_s = small_12_x-small_02_x;

  int small_03_x = space(ww,200,small_s,3,0);
  int small_13_x = space(ww,200,small_s,3,1);
  //int small_23_x = space(ww,200,small_s,3,2);
  //int small_x3_s = small_13_x-small_03_x;

  int small_04_x = space(ww,200,small_s,4,0);
  int small_14_x = space(ww,200,small_s,4,1);
  int small_24_x = space(ww,200,small_s,4,2);
  int small_34_x = space(ww,200,small_s,4,3);
  //int small_x4_s = small_14_x-small_04_x;

  int small_05_x = space(ww,200,small_s,5,0);
  int small_15_x = space(ww,200,small_s,5,1);
  int small_25_x = space(ww,200,small_s,5,2);
  int small_35_x = space(ww,200,small_s,5,3);
  int small_45_x = space(ww,200,small_s,5,4);
  //int small_x5_s = small_15_x-small_05_x;

  //int big_01_x = space(ww,200,big_s,1,0);

  int big_02_x = space(ww,200,big_s,2,0);
  int big_12_x = space(ww,200,big_s,2,1);
  //int big_x2_s = big_12_x-big_02_x;

  int big_03_x = space(ww,200,big_s,3,0);
  int big_13_x = space(ww,200,big_s,3,1);
  int big_23_x = space(ww,200,big_s,3,2);
  //int big_x3_s = big_13_x-big_03_x;

  int big_04_x = space(ww,200,big_s,4,0);
  int big_14_x = space(ww,200,big_s,4,1);
  int big_24_x = space(ww,200,big_s,4,2);
  int big_34_x = space(ww,200,big_s,4,3);
  //int big_x4_s = big_14_x-big_04_x;

  int big_05_x = space(ww,200,big_s,5,0);
  int big_15_x = space(ww,200,big_s,5,1);
  int big_25_x = space(ww,200,big_s,5,2);
  int big_35_x = space(ww,200,big_s,5,3);
  int big_45_x = space(ww,200,big_s,5,4);
  //int big_x5_s = big_15_x-big_05_x;

  UI::TextButton cancel_single_button("cancel",  space(ww,200,100,1,0),350,100,30);
  UI::TextButton cancel_double_button("cancel",  space(ww,200,100,2,0),350,100,30);
  UI::TextButton confirm_double_button("confirm",space(ww,200,100,2,1),350,100,30);

  UI::Button whoButtonCW(rectForPosition('w'));
  UI::Button whoButtonCCW(rectForPosition('e'));

  //choose_ (what)
  choose_attack_image   = UI::ImageButtonRound(sword_s,       big_04_x, big_y, big_s, big_s);
  choose_defend_image   = UI::ImageButtonRound(shield_full_s, big_14_x, big_y, big_s, big_s);
  choose_message_image  = UI::ImageButtonRound(envelope_s,    big_24_x, big_y, big_s, big_s);
  choose_sabotage_image = UI::ImageButtonRound(red_x_s,       big_34_x, big_y, big_s, big_s);

  //choose_attack_ (who)
  choose_attack_attack_image   = UI::ImageButtonRound(sword_s, small_02_x, small_y, small_s, small_s);
  choose_attack_who_image      = UI::ImageButtonRound(who_ns_s,   big_12_x,   big_y,   big_s,   big_s);
  choose_attack_who_button_cw  = whoButtonCW;
  choose_attack_who_button_ccw = whoButtonCCW;
  choose_attack_cancel_button  = cancel_single_button;

  //choose_attack_who (confirm)
  choose_attack_who_attack_image   = UI::ImageButtonRound(sword_s, small_02_x, small_y, small_s, small_s);
  choose_attack_who_who_image      = UI::ImageButtonRound(who_ns_s, small_12_x, small_y, small_s, small_s);
  choose_attack_who_cancel_button  = cancel_double_button;
  choose_attack_who_confirm_button = confirm_double_button;

  //choose_defend (confirm)
  choose_defend_defend_image   = UI::ImageButtonRound(shield_full_s, small_01_x, small_y, small_s, small_s);
  choose_defend_cancel_button  = cancel_double_button;
  choose_defend_confirm_button = confirm_double_button;

  //choose_message_ (what)
  choose_message_message_image = UI::ImageButtonRound(envelope_s,  small_03_x, small_y, small_s, small_s);
  choose_message_attack_image  = UI::ImageButtonRound(sword_s,       big_13_x, big_y, big_s, big_s);
  choose_message_defend_image  = UI::ImageButtonRound(shield_full_s, big_23_x, big_y, big_s, big_s);
  choose_message_cancel_button = cancel_single_button;

  //choose_message_attack_ (who)
  choose_message_attack_message_image  = UI::ImageButtonRound(envelope_s, small_03_x, small_y, small_s, small_s);
  choose_message_attack_attack_image   = UI::ImageButtonRound(sword_s,    small_13_x, small_y, small_s, small_s);
  choose_message_attack_who_image      = UI::ImageButtonRound(who_ns_s,       big_23_x,   big_y,   big_s,   big_s);
  choose_message_attack_who_button_cw  = whoButtonCW;
  choose_message_attack_who_button_ccw = whoButtonCCW;
  choose_message_attack_cancel_button         = cancel_single_button;

  //choose_message_attack_who_ (when)
  choose_message_attack_who_message_image = UI::ImageButtonRound(envelope_s, small_04_x, small_y, small_s, small_s);
  choose_message_attack_who_attack_image  = UI::ImageButtonRound(sword_s,    small_14_x, small_y, small_s, small_s);
  choose_message_attack_who_who_image     = UI::ImageButtonRound(who_ns_s,     small_24_x, small_y, small_s, small_s);
  choose_message_attack_who_when_image    = UI::ImageButtonRound(when_s,       big_34_x,   big_y,   big_s,   big_s);
  for(int i = 0; i < 7; i++)
    choose_message_attack_who_when_buttons[i] = UI::Button(dayRects[i]);
  choose_message_attack_who_cancel_button = cancel_single_button;

  //choose_message_attack_who_when_ (route)
  choose_message_attack_who_when_message_image = UI::ImageButtonRound(envelope_s, small_05_x, small_y, small_s, small_s);
  choose_message_attack_who_when_attack_image  = UI::ImageButtonRound(sword_s,    small_15_x, small_y, small_s, small_s);
  choose_message_attack_who_when_who_image     = UI::ImageButtonRound(who_ns_s,     small_25_x, small_y, small_s, small_s);
  choose_message_attack_who_when_when_image    = UI::ImageButtonRound(when_s,     small_35_x, small_y, small_s, small_s);
  choose_message_attack_who_when_route_image   = UI::ImageButtonRound(where_ns_s,       big_45_x,   big_y,   big_s,   big_s);
  choose_message_attack_who_when_route_button_cw  = whoButtonCW;
  choose_message_attack_who_when_route_button_ccw = whoButtonCCW;
  choose_message_attack_who_when_cancel_button = cancel_single_button;

  //choose_message_attack_who_when_route (confirm)
  choose_message_attack_who_when_route_message_image = UI::ImageButtonRound(envelope_s, small_05_x, small_y, small_s, small_s);
  choose_message_attack_who_when_route_attack_image  = UI::ImageButtonRound(sword_s,    small_15_x, small_y, small_s, small_s);
  choose_message_attack_who_when_route_who_image     = UI::ImageButtonRound(who_ns_s,     small_25_x, small_y, small_s, small_s);
  choose_message_attack_who_when_route_when_image    = UI::ImageButtonRound(when_s,     small_35_x, small_y, small_s, small_s);
  choose_message_attack_who_when_route_route_image   = UI::ImageButtonRound(where_ns_s,     small_45_x, small_y, small_s, small_s);
  choose_message_attack_who_when_route_cancel_button = cancel_double_button;
  choose_message_attack_who_when_route_confirm_button = confirm_double_button;

  //choose_message_defend_ (when)
  choose_message_defend_message_image = UI::ImageButtonRound(envelope_s,    small_03_x, small_y, small_s, small_s);
  choose_message_defend_defend_image  = UI::ImageButtonRound(shield_full_s, small_13_x, small_y, small_s, small_s);
  choose_message_defend_when_image    = UI::ImageButtonRound(when_s,          big_23_x,   big_y,   big_s,   big_s);
  for(int i = 0; i < 7; i++)
    choose_defend_when_buttons[i] = UI::Button(dayRects[i]);
  choose_message_defend_cancel_button = cancel_single_button;

  //choose_message_defend_when_ (route)
  choose_message_defend_when_message_image = UI::ImageButtonRound(envelope_s,    small_04_x, small_y, small_s, small_s);
  choose_message_defend_when_defend_image  = UI::ImageButtonRound(shield_full_s, small_14_x, small_y, small_s, small_s);
  choose_message_defend_when_when_image    = UI::ImageButtonRound(when_s,        small_24_x, small_y, small_s, small_s);
  choose_message_defend_when_route_image   = UI::ImageButtonRound(where_ns_s,          big_34_x,   big_y,   big_s,   big_s);
  choose_message_defend_when_route_button_cw  = whoButtonCW;
  choose_message_defend_when_route_button_ccw = whoButtonCCW;
  choose_message_defend_when_cancel_button = cancel_single_button;

  //choose_message_defend_when_route (confirm)
  choose_message_defend_when_route_message_image = UI::ImageButtonRound(envelope_s,    small_04_x, small_y, small_s, small_s);
  choose_message_defend_when_route_defend_image  = UI::ImageButtonRound(shield_full_s, small_14_x, small_y, small_s, small_s);
  choose_message_defend_when_route_when_image    = UI::ImageButtonRound(when_s,        small_24_x, small_y, small_s, small_s);
  choose_message_defend_when_route_route_image   = UI::ImageButtonRound(where_ns_s,        small_34_x, small_y, small_s, small_s);
  choose_message_defend_when_route_cancel_button  = cancel_double_button;
  choose_message_defend_when_route_confirm_button = confirm_double_button;

  //choose_sabotage_ (how)
  choose_sabotage_sabotage_image = UI::ImageButtonRound(red_x_s, small_04_x, small_y, small_s, small_s);
  choose_sabotage_block_image    = UI::ImageButtonRound(sblock_s,    big_14_x,   big_y,   big_s,   big_s);
  choose_sabotage_read_image     = UI::ImageButtonRound(sread_s,    big_24_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_image   = UI::ImageButtonRound(sswitch_s,    big_34_x,   big_y,   big_s,   big_s);
  choose_sabotage_cancel_button  = cancel_single_button;

  //choose_sabotage_block (confirm)
  choose_sabotage_block_sabotage_image = UI::ImageButtonRound(red_x_s, small_02_x, small_y, small_s, small_s);
  choose_sabotage_block_block_image    = UI::ImageButtonRound(sblock_s,  small_12_x, small_y, small_s, small_s);
  choose_sabotage_block_cancel_button  = cancel_double_button;
  choose_sabotage_block_confirm_button = confirm_double_button;

  //choose_sabotage_read (confirm)
  choose_sabotage_read_sabotage_image = UI::ImageButtonRound(red_x_s, small_02_x, small_y, small_s, small_s);
  choose_sabotage_read_read_image     = UI::ImageButtonRound(sread_s,  small_12_x, small_y, small_s, small_s);
  choose_sabotage_read_cancel_button  = cancel_double_button;
  choose_sabotage_read_confirm_button = confirm_double_button;

  //choose_sabotage_switch_ (what)
  choose_sabotage_switch_sabotage_image = UI::ImageButtonRound(red_x_s, small_05_x, small_y, small_s, small_s);
  choose_sabotage_switch_switch_image   = UI::ImageButtonRound(sswitch_s,  small_15_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_image     = UI::ImageButtonRound(what_s,    big_25_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_whot_image     = UI::ImageButtonRound(who_ns_s,    big_35_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_whent_image    = UI::ImageButtonRound(when_s,    big_45_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_cancel_button  = cancel_single_button;

  //choose_sabotage_switch_what_ (what)
  choose_sabotage_switch_what_sabotage_image = UI::ImageButtonRound(red_x_s,       small_05_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_switch_image   = UI::ImageButtonRound(sswitch_s,        small_15_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_what_image     = UI::ImageButtonRound(what_s,        small_25_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_attack_image   = UI::ImageButtonRound(sword_s,         big_35_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_what_defend_image   = UI::ImageButtonRound(shield_full_s,   big_45_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_what_cancel_button  = cancel_single_button;

  //choose_sabotage_switch_what_attack (confirm)
  choose_sabotage_switch_what_attack_sabotage_image = UI::ImageButtonRound(red_x_s, small_04_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_attack_switch_image   = UI::ImageButtonRound(sswitch_s,  small_14_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_attack_what_image     = UI::ImageButtonRound(what_s,  small_24_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_attack_attack_image   = UI::ImageButtonRound(sword_s, small_34_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_attack_cancel_button  = cancel_double_button;
  choose_sabotage_switch_what_attack_confirm_button = confirm_double_button;

  //choose_sabotage_switch_what_defend (confirm)
  choose_sabotage_switch_what_defend_sabotage_image = UI::ImageButtonRound(red_x_s,       small_04_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_defend_switch_image   = UI::ImageButtonRound(sswitch_s,        small_14_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_defend_what_image     = UI::ImageButtonRound(what_s,        small_24_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_defend_defend_image   = UI::ImageButtonRound(shield_full_s, small_34_x, small_y, small_s, small_s);
  choose_sabotage_switch_what_defend_cancel_button  = cancel_double_button;
  choose_sabotage_switch_what_defend_confirm_button = confirm_double_button;

  //choose_sabotage_switch_whot_ (who)
  choose_sabotage_switch_whot_sabotage_image = UI::ImageButtonRound(red_x_s, small_04_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_switch_image   = UI::ImageButtonRound(sswitch_s,  small_14_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_whot_image     = UI::ImageButtonRound(who_ns_s,  small_24_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_who_image      = UI::ImageButtonRound(who_ns_s,    big_34_x,   big_y,   big_s,   big_s);
  choose_sabotage_switch_whot_who_button_cw  = whoButtonCW;
  choose_sabotage_switch_whot_who_button_ccw = whoButtonCCW;
  choose_sabotage_switch_whot_cancel_button  = cancel_single_button;

  //choose_sabotage_switch_whot_who (confirm)
  choose_sabotage_switch_whot_who_sabotage_image = UI::ImageButtonRound(red_x_s, small_04_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_who_switch_image   = UI::ImageButtonRound(sswitch_s,  small_14_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_who_whot_image     = UI::ImageButtonRound(who_ns_s,  small_24_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_who_who_image      = UI::ImageButtonRound(who_ns_s,  small_34_x, small_y, small_s, small_s);
  choose_sabotage_switch_whot_who_cancel_button  = cancel_double_button;
  choose_sabotage_switch_whot_who_confirm_button = confirm_double_button;

  //choose_sabotage_switch_whent_ (when)
  choose_sabotage_switch_whent_sabotage_image = UI::ImageButtonRound(red_x_s, small_04_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_switch_image   = UI::ImageButtonRound(sswitch_s,  small_14_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_whent_image    = UI::ImageButtonRound(when_s,  small_24_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_when_image     = UI::ImageButtonRound(when_s,    big_34_x,   big_y,   big_s,   big_s);
  for(int i = 0; i < 7; i++)
    choose_sabotage_switch_whent_when_buttons[i] = UI::Button(dayRects[i]);
  choose_sabotage_switch_whent_cancel_button = cancel_single_button;

  //choose_sabotage_switch_whent_when (confirm)
  choose_sabotage_switch_whent_when_sabotage_image = UI::ImageButtonRound(red_x_s, small_04_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_when_switch_image   = UI::ImageButtonRound(sswitch_s,  small_14_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_when_whent_image    = UI::ImageButtonRound(when_s,  small_24_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_when_when_image     = UI::ImageButtonRound(when_s,  small_34_x, small_y, small_s, small_s);
  choose_sabotage_switch_whent_when_cancel_button  = cancel_double_button;
  choose_sabotage_switch_whent_when_confirm_button = confirm_double_button;

  loading = UI::Anim(UI::AnimSprites(Sprite::loading_0(), Sprite::loading_1(), Sprite::loading_2(), Sprite::loading_2()), 3, 1.f, ww/2-250, wh/2-120,40,40);
  waiting_on_players_label = UI::Label("waiting on players...", wh/2-20, 200, 40);
  reset_game_button        = UI::TextButton("reset game", ww/2-100, wh/2-20, 200, 40);
  win_img  = UI::Image(sun_s,   ww/2-100, wh/2-100, 200, 200);
  lose_img = UI::Image(red_x_s, ww/2-100, wh/2-100, 200, 200);
  tie_img  = UI::Image(sun_s,   ww/2-100, wh/2-100, 200, 200);

  known_day = '0';
  anim_day = 0.0f;
  shown_day = 0.0f;
}

void PlayScene::enter()
{
  //need to wait until c_model is obtained
  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;

  char card;
  int icard;
  SDL_Rect rect;

  //bottom (you)
  card = c->myCardinal();
  icard = Compass::icardinal(card);
  rect = rectForPosition('s');
  cardImgs[icard] = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;

  //left (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('w');
  cardImgs[icard]  = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('n');
  cardImgs[icard]  = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('e');
  cardImgs[icard]  = UI::Anim(generals_s[icard], 4, 1.f, rect);
  cardLbls[icard].rect = rect; cardLbls[icard].rect.y += cardLbls[icard].rect.h; cardLbls[icard].rect.h /= 3;
}

void PlayScene::zeroE()
{
  e.zero();
  e.connection = c->myConnection();
  e.cardinal = c->myCardinal();
  e.type = e_type_commit_action;
}

void PlayScene::chooseShownDay(In &in)
{
  if(sunBtn.query(in)) sunDragging = true;

  if(sunDragging)
  {
    int firstX = dayRects[0].x+(dayRects[0].w/2);
    int lastX  = dayRects[6].x+(dayRects[6].w/2);
    shown_day = ((float)(in.x-firstX)/(float)(lastX-firstX))*6.0f;
  }
}

void PlayScene::chooseReadSabotage(In &in)
{
  if(read_sabotage_0.query(in)) sabotage_0_reading = true;
  if(read_sabotage_1.query(in)) sabotage_1_reading = true;
}

void PlayScene::chooseReadMessage(In &in)
{
  if(read_message.query(in)) message_reading = true;
}

void PlayScene::chooseWhat(In &in)
{
  if(choose_attack_image.query(in))    e.action = 'a';
  if(choose_defend_image.query(in))    e.action = 'd';
  if(choose_message_image.query(in)) { e.action = 'm'; e.to = Compass::opcardinal(e.cardinal); } //auto assign 'to'
  if(choose_sabotage_image.query(in))  e.action = 's';
}

void PlayScene::chooseAttackWho(In &in)
{
  if(choose_attack_attack_image.query(in)) zeroE();
  if(choose_attack_who_image.query(in)) /*nothing*/{};
  if(choose_attack_who_button_cw.query(in))  e.who = Compass::cwcardinal(e.cardinal);
  if(choose_attack_who_button_ccw.query(in)) e.who = Compass::ccwcardinal(e.cardinal);
  if(choose_attack_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseAttackWhoConfirm(In &in)
{
  if(choose_attack_who_attack_image.query(in)) zeroE();
  if(choose_attack_who_who_image.query(in)) e.who = '0';
  if(choose_attack_who_cancel_button.query(in)) zeroE();
  if(choose_attack_who_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseDefendConfirm(In &in)
{
  if(choose_defend_defend_image.query(in)) zeroE();
  if(choose_defend_cancel_button.query(in)) zeroE();
  if(choose_defend_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseMessageWhat(In &in)
{
  if(choose_message_message_image.query(in)) zeroE();
  if(choose_message_attack_image.query(in)) e.what = 'a';
  if(choose_message_defend_image.query(in)) e.what = 'd';
  if(choose_message_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseMessageAttackWho(In &in)
{
  if(choose_message_attack_message_image.query(in)) zeroE();
  if(choose_message_attack_attack_image.query(in)) e.what = '0';
  if(choose_message_attack_who_image.query(in)) /* nothing */{};
  if(choose_message_attack_who_button_cw.query(in))  e.who = Compass::cwcardinal(e.cardinal);
  if(choose_message_attack_who_button_ccw.query(in)) e.who = Compass::cwcardinal(e.cardinal);
  if(choose_message_attack_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseMessageAttackWhoWhen(In &in)
{
  if(choose_message_attack_who_message_image.query(in)) zeroE();
  if(choose_message_attack_who_attack_image.query(in)) { e.who = '0'; e.what = '0'; }
  if(choose_message_attack_who_who_image.query(in)) e.who = '0';
  if(choose_message_attack_who_when_image.query(in)) /* nothing */{};
  for(int i = 0; i < 7; i++)
    if(choose_message_attack_who_when_buttons[i].query(in)) e.when = Week::day(i);
  if(choose_message_attack_who_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseMessageAttackWhoWhenRoute(In &in)
{
  if(choose_message_attack_who_when_message_image.query(in)) zeroE();
  if(choose_message_attack_who_when_attack_image.query(in)) { e.when = '0'; e.who = '0'; e.what = '0'; }
  if(choose_message_attack_who_when_who_image.query(in)) { e.when = '0'; e.who = '0'; };
  if(choose_message_attack_who_when_when_image.query(in)) e.when = '0';
  if(choose_message_attack_who_when_route_image.query(in)) /* nothing */{};
  if(choose_message_attack_who_when_route_button_cw.query(in))  e.route = Compass::cwcardinal(e.cardinal);
  if(choose_message_attack_who_when_route_button_ccw.query(in)) e.route = Compass::ccwcardinal(e.cardinal);
  if(choose_message_attack_who_when_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseMessageAttackWhoWhenRouteConfirm(In &in)
{
  if(choose_message_attack_who_when_route_message_image.query(in)) zeroE();
  if(choose_message_attack_who_when_route_attack_image.query(in)) { e.route = '0'; e.when = '0'; e.who = '0'; e.what = '0'; };
  if(choose_message_attack_who_when_route_who_image.query(in)) { e.route = '0'; e.when = '0'; e.who = '0'; };
  if(choose_message_attack_who_when_route_when_image.query(in)) { e.route = '0'; e.when = '0'; };
  if(choose_message_attack_who_when_route_route_image.query(in)) e.route = '0';
  if(choose_message_attack_who_when_route_cancel_button.query(in)) zeroE();
  if(choose_message_attack_who_when_route_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseMessageDefendWhen(In &in)
{
  if(choose_message_defend_message_image.query(in)) zeroE();
  if(choose_message_defend_defend_image.query(in)) e.what = '0';
  if(choose_message_defend_when_image.query(in)) /* nothing */{};
  for(int i = 0; i < 7; i++)
    if(choose_defend_when_buttons[i].query(in)) e.when = Week::day(i);
  if(choose_message_defend_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseMessageDefendWhenRoute(In &in)
{
  if(choose_message_defend_when_message_image.query(in)) zeroE();
  if(choose_message_defend_when_defend_image.query(in)) { e.when = '0'; e.what = '0'; };
  if(choose_message_defend_when_when_image.query(in)) e.when = '0';
  if(choose_message_defend_when_route_image.query(in)) /* nothing */{};
  if(choose_message_defend_when_route_button_cw.query(in))  e.route = Compass::cwcardinal(e.cardinal);
  if(choose_message_defend_when_route_button_ccw.query(in)) e.route = Compass::ccwcardinal(e.cardinal);
  if(choose_message_defend_when_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseMessageDefendWhenRouteConfirm(In &in)
{
  if(choose_message_defend_when_route_message_image.query(in)) zeroE();
  if(choose_message_defend_when_route_defend_image.query(in)) { e.route = '0'; e.when = '0'; e.what = '0'; };
  if(choose_message_defend_when_route_when_image.query(in)) { e.route = '0'; e.when = '0'; };
  if(choose_message_defend_when_route_route_image.query(in)) e.route = '0';
  if(choose_message_defend_when_route_cancel_button.query(in)) zeroE();
  if(choose_message_defend_when_route_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseSabotageHow(In &in)
{
  if(choose_sabotage_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_block_image.query(in)) e.how = 'b';
  if(choose_sabotage_read_image.query(in)) e.how = 'r';
  if(choose_sabotage_switch_image.query(in)) e.how = 's';
  if(choose_sabotage_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseSabotageBlockConfirm(In &in)
{
  if(choose_sabotage_block_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_block_block_image.query(in)) e.how = '0';
  if(choose_sabotage_block_cancel_button.query(in)) zeroE();
  if(choose_sabotage_block_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseSabotageReadConfirm(In &in)
{
  if(choose_sabotage_read_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_read_read_image.query(in)) e.how = '0';
  if(choose_sabotage_read_cancel_button.query(in)) zeroE();
  if(choose_sabotage_read_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseSabotageSwitchWhat(In &in)
{
  if(choose_sabotage_switch_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_switch_image.query(in)) e.how = '0';
  if(choose_sabotage_switch_what_image.query(in)) e.which = 'a';
  if(choose_sabotage_switch_whot_image.query(in))  e.which = 'o';
  if(choose_sabotage_switch_whent_image.query(in)) e.which = 'e';
  if(choose_sabotage_switch_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseSabotageSwitchWhatWhat(In &in)
{
  if(choose_sabotage_switch_what_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_what_switch_image.query(in)) { e.which = '0'; e.how = '0'; };
  if(choose_sabotage_switch_what_what_image.query(in)) e.which = '0';
  if(choose_sabotage_switch_what_attack_image.query(in)) e.what = 'a';
  if(choose_sabotage_switch_what_defend_image.query(in)) e.what = 'd';
  if(choose_sabotage_switch_what_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseSabotageSwitchWhatAttackConfirm(In &in)
{
  if(choose_sabotage_switch_what_attack_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_what_attack_switch_image.query(in)) { e.what = '0'; e.which = '0'; e.how = '0'; };
  if(choose_sabotage_switch_what_attack_what_image.query(in)) { e.what = '0'; e.which = '0'; };
  if(choose_sabotage_switch_what_attack_attack_image.query(in)) e.what = '0';
  if(choose_sabotage_switch_what_attack_cancel_button.query(in)) zeroE();
  if(choose_sabotage_switch_what_attack_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseSabotageSwitchWhatDefendConfirm(In &in)
{
  if(choose_sabotage_switch_what_defend_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_what_defend_switch_image.query(in)) { e.what = '0'; e.which = '0'; e.how = '0'; };
  if(choose_sabotage_switch_what_defend_what_image.query(in)) { e.what = '0'; e.which = '0'; };
  if(choose_sabotage_switch_what_defend_defend_image.query(in)) e.what = '0';
  if(choose_sabotage_switch_what_defend_cancel_button.query(in)) zeroE();
  if(choose_sabotage_switch_what_defend_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseSabotageSwitchWhoWho(In &in)
{
  if(choose_sabotage_switch_whot_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_whot_switch_image.query(in)) { e.which = '0'; e.how = '0'; }
  if(choose_sabotage_switch_whot_whot_image.query(in)) e.which = '0';;
  if(choose_sabotage_switch_whot_who_image.query(in)) /* nothing */{};
  if(choose_sabotage_switch_whot_who_button_cw.query(in))  e.who = Compass::cwcardinal(e.cardinal);
  if(choose_sabotage_switch_whot_who_button_ccw.query(in)) e.who = Compass::ccwcardinal(e.cardinal);
  if(choose_sabotage_switch_whot_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseSabotageSwitchWhoWhoConfirm(In &in)
{
  if(choose_sabotage_switch_whot_who_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_whot_who_switch_image.query(in)) { e.who = '0'; e.which = '0'; e.how = '0'; };
  if(choose_sabotage_switch_whot_who_whot_image.query(in)) { e.who = '0'; e.which = '0'; };
  if(choose_sabotage_switch_whot_who_who_image.query(in)) e.who = '0';
  if(choose_sabotage_switch_whot_who_cancel_button.query(in)) zeroE();
  if(choose_sabotage_switch_whot_who_confirm_button.query(in)) c->commitAction(e);
}

void PlayScene::chooseSabotageSwitchWhenWhen(In &in)
{
  if(choose_sabotage_switch_whent_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_whent_switch_image.query(in)) { e.which = '0'; e.how = '0'; };
  if(choose_sabotage_switch_whent_whent_image.query(in)) e.which = '0';
  if(choose_sabotage_switch_whent_when_image.query(in)) /* nothing */{};
  for(int i = 0; i < 7; i++)
    if(choose_sabotage_switch_whent_when_buttons[i].query(in)) e.when = Week::day(i);
  if(choose_sabotage_switch_whent_cancel_button.query(in)) zeroE();
}

void PlayScene::chooseSabotageSwitchWhenWhenConfirm(In &in)
{
  if(choose_sabotage_switch_whent_when_sabotage_image.query(in)) zeroE();
  if(choose_sabotage_switch_whent_when_switch_image.query(in)) { e.when = '0'; e.which = '0'; e.how = '0'; };
  if(choose_sabotage_switch_whent_when_whent_image.query(in)) { e.when = '0'; e.which = '0'; }
  if(choose_sabotage_switch_whent_when_when_image.query(in)) e.when = '0';
  if(choose_sabotage_switch_whent_when_cancel_button.query(in)) zeroE();
  if(choose_sabotage_switch_whent_when_confirm_button.query(in)) c->commitAction(e);
}







void PlayScene::drawSabotage0()
{
  read_sabotage_0.draw(graphics);
}

void PlayScene::drawSabotage1()
{
  read_sabotage_1.draw(graphics);
}

void PlayScene::drawMessage()
{
  read_message.draw(graphics);
}

void PlayScene::drawWhat()
{
  choose_attack_image.draw(graphics);
  choose_defend_image.draw(graphics);
  choose_message_image.draw(graphics);
  choose_sabotage_image.draw(graphics);
}

void PlayScene::drawAttackWho()
{
  choose_attack_attack_image.draw(graphics);
  choose_attack_who_image.draw(graphics);
  choose_attack_cancel_button.draw(graphics);
}

void PlayScene::drawAttackWhoConfirm()
{
  choose_attack_who_attack_image.draw(graphics);
  choose_attack_who_who_image.sprite = generals_s[Compass::icardinal(e.who)].sprite[0];
  choose_attack_who_who_image.draw(graphics);
  choose_attack_who_cancel_button.draw(graphics);
  choose_attack_who_confirm_button.draw(graphics);
}

void PlayScene::drawDefendConfirm()
{
  choose_defend_defend_image.draw(graphics);
  choose_defend_cancel_button.draw(graphics);
  choose_defend_confirm_button.draw(graphics);
}

void PlayScene::drawMessageWhat()
{
  choose_message_message_image.draw(graphics);
  choose_message_attack_image.draw(graphics);
  choose_message_defend_image.draw(graphics);
  choose_message_cancel_button.draw(graphics);
}

void PlayScene::drawMessageAttackWho()
{
  choose_message_attack_message_image.draw(graphics);
  choose_message_attack_attack_image.draw(graphics);
  choose_message_attack_who_image.draw(graphics);
  choose_message_attack_cancel_button.draw(graphics);
}

void PlayScene::drawMessageAttackWhoWhen()
{
  choose_message_attack_who_message_image.draw(graphics);
  choose_message_attack_who_attack_image.draw(graphics);
  choose_message_attack_who_who_image.sprite = generals_s[Compass::icardinal(e.who)].sprite[0];
  choose_message_attack_who_who_image.draw(graphics);
  choose_message_attack_who_when_image.draw(graphics);
  choose_message_attack_who_cancel_button.draw(graphics);
}

void PlayScene::drawMessageAttackWhoWhenRoute()
{
  choose_message_attack_who_when_message_image.draw(graphics);
  choose_message_attack_who_when_attack_image.draw(graphics);
  choose_message_attack_who_when_who_image.sprite = generals_s[Compass::icardinal(e.who)].sprite[0];
  choose_message_attack_who_when_who_image.draw(graphics);
  choose_message_attack_who_when_when_image.draw(graphics);
  choose_message_attack_who_when_route_image.draw(graphics);
  choose_message_attack_who_when_cancel_button.draw(graphics);
}

void PlayScene::drawMessageAttackWhoWhenRouteConfirm()
{
  choose_message_attack_who_when_route_message_image.draw(graphics);
  choose_message_attack_who_when_route_attack_image.draw(graphics);
  choose_message_attack_who_when_route_who_image.sprite = generals_s[Compass::icardinal(e.who)].sprite[0];
  choose_message_attack_who_when_route_who_image.draw(graphics);
  choose_message_attack_who_when_route_when_image.draw(graphics);
  choose_message_attack_who_when_route_route_image.draw(graphics);
  choose_message_attack_who_when_route_cancel_button.draw(graphics);
  choose_message_attack_who_when_route_confirm_button.draw(graphics);
}

void PlayScene::drawMessageDefendWhen()
{
  choose_message_defend_message_image.draw(graphics);
  choose_message_defend_defend_image.draw(graphics);
  choose_message_defend_when_image.draw(graphics);
  choose_message_defend_cancel_button.draw(graphics);
}

void PlayScene::drawMessageDefendWhenRoute()
{
  choose_message_defend_when_message_image.draw(graphics);
  choose_message_defend_when_defend_image.draw(graphics);
  choose_message_defend_when_when_image.draw(graphics);
  choose_message_defend_when_route_image.draw(graphics);
  choose_message_defend_when_cancel_button.draw(graphics);
}

void PlayScene::drawMessageDefendWhenRouteConfirm()
{
  choose_message_defend_when_route_message_image.draw(graphics);
  choose_message_defend_when_route_defend_image.draw(graphics);
  choose_message_defend_when_route_when_image.draw(graphics);
  choose_message_defend_when_route_route_image.draw(graphics);
  choose_message_defend_when_route_cancel_button.draw(graphics);
  choose_message_defend_when_route_confirm_button.draw(graphics);
}

void PlayScene::drawSabotageHow()
{
  choose_sabotage_sabotage_image.draw(graphics);
  choose_sabotage_block_image.draw(graphics);
  choose_sabotage_read_image.draw(graphics);
  choose_sabotage_switch_image.draw(graphics);
  choose_sabotage_cancel_button.draw(graphics);
}

void PlayScene::drawSabotageBlockConfirm()
{
  choose_sabotage_block_sabotage_image.draw(graphics);
  choose_sabotage_block_block_image.draw(graphics);
  choose_sabotage_block_cancel_button.draw(graphics);
  choose_sabotage_block_confirm_button.draw(graphics);
}

void PlayScene::drawSabotageReadConfirm()
{
  choose_sabotage_read_sabotage_image.draw(graphics);
  choose_sabotage_read_read_image.draw(graphics);
  choose_sabotage_read_cancel_button.draw(graphics);
  choose_sabotage_read_confirm_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhat()
{
  choose_sabotage_switch_sabotage_image.draw(graphics);
  choose_sabotage_switch_switch_image.draw(graphics);
  choose_sabotage_switch_what_image.draw(graphics);
  choose_sabotage_switch_whot_image.draw(graphics);
  choose_sabotage_switch_whent_image.draw(graphics);
  choose_sabotage_switch_cancel_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhatWhat()
{
  choose_sabotage_switch_what_sabotage_image.draw(graphics);
  choose_sabotage_switch_what_switch_image.draw(graphics);
  choose_sabotage_switch_what_what_image.draw(graphics);
  choose_sabotage_switch_what_attack_image.draw(graphics);
  choose_sabotage_switch_what_defend_image.draw(graphics);
  choose_sabotage_switch_what_cancel_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhatAttackConfirm()
{
  choose_sabotage_switch_what_attack_sabotage_image.draw(graphics);
  choose_sabotage_switch_what_attack_switch_image.draw(graphics);
  choose_sabotage_switch_what_attack_what_image.draw(graphics);
  choose_sabotage_switch_what_attack_attack_image.draw(graphics);
  choose_sabotage_switch_what_attack_cancel_button.draw(graphics);
  choose_sabotage_switch_what_attack_confirm_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhatDefendConfirm()
{
  choose_sabotage_switch_what_defend_sabotage_image.draw(graphics);
  choose_sabotage_switch_what_defend_switch_image.draw(graphics);
  choose_sabotage_switch_what_defend_what_image.draw(graphics);
  choose_sabotage_switch_what_defend_defend_image.draw(graphics);
  choose_sabotage_switch_what_defend_cancel_button.draw(graphics);
  choose_sabotage_switch_what_defend_confirm_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhoWho()
{
  choose_sabotage_switch_whot_sabotage_image.draw(graphics);
  choose_sabotage_switch_whot_switch_image.draw(graphics);
  choose_sabotage_switch_whot_whot_image.draw(graphics);
  choose_sabotage_switch_whot_who_image.draw(graphics);
  choose_sabotage_switch_whot_cancel_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhoWhoConfirm()
{
  choose_sabotage_switch_whot_who_sabotage_image.draw(graphics);
  choose_sabotage_switch_whot_who_switch_image.draw(graphics);
  choose_sabotage_switch_whot_who_whot_image.draw(graphics);
  choose_sabotage_switch_whot_who_who_image.sprite = generals_s[Compass::icardinal(e.who)].sprite[0];
  choose_sabotage_switch_whot_who_who_image.draw(graphics);
  choose_sabotage_switch_whot_who_cancel_button.draw(graphics);
  choose_sabotage_switch_whot_who_confirm_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhenWhen()
{
  choose_sabotage_switch_whent_sabotage_image.draw(graphics);
  choose_sabotage_switch_whent_switch_image.draw(graphics);
  choose_sabotage_switch_whent_whent_image.draw(graphics);
  choose_sabotage_switch_whent_when_image.draw(graphics);
  choose_sabotage_switch_whent_cancel_button.draw(graphics);
}

void PlayScene::drawSabotageSwitchWhenWhenConfirm()
{
  choose_sabotage_switch_whent_when_sabotage_image.draw(graphics);
  choose_sabotage_switch_whent_when_switch_image.draw(graphics);
  choose_sabotage_switch_whent_when_whent_image.draw(graphics);
  choose_sabotage_switch_whent_when_when_image.draw(graphics);
  choose_sabotage_switch_whent_when_cancel_button.draw(graphics);
  choose_sabotage_switch_whent_when_confirm_button.draw(graphics);
}

void PlayScene::drawWaiting()
{
  loading.draw(graphics);
  waiting_on_players_label.draw(graphics);
}

void PlayScene::drawReset()
{
  reset_game_button.draw(graphics);
}

void PlayScene::drawWin()
{
  win_img.draw(graphics);
}

void PlayScene::drawLose()
{
  lose_img.draw(graphics);
}

void PlayScene::drawTie()
{
  tie_img.draw(graphics);
}















void PlayScene::touch(In &in)
{
  if(in.type == In::UP)
  {
    sunDragging = false;
    sabotage_0_reading = false;
    sabotage_1_reading = false;
    message_reading = false;
  }

  if((in.type == In::DOWN || in.type == In::MOVE) && !c->iHaveAction() && e.action == '0')
    chooseShownDay(in);

  if(in.type == In::DOWN)
  {
    Messenger m;
    Messenger m0;
    Messenger m1;
    if(c->myMessage(m))
    {
      chooseReadMessage(in);
    }
    if(c->mySabotage(m0, m1))
    {
      chooseReadSabotage(in);
    }
  }

  //oh god terrible tree traversal touch propagation
  if(in.type == In::DOWN)
  {
    if(!c->iWin() && !c->iLose() && !c->iTie())
    {
      if(!c->iHaveAction())
      {
        if(e.action == '0')
        {
          chooseWhat(in);
        }
        else if(e.action == 'a')
        {
          if(e.who == '0')
          {
            chooseAttackWho(in);
          }
          else
          {
            chooseAttackWhoConfirm(in);
          }
        }
        else if(e.action == 'd')
        {
          chooseDefendConfirm(in);
        }
        else if(e.action == 'm')
        {
          if(e.what == '0')
          {
            chooseMessageWhat(in);
          }
          else if(e.what == 'a')
          {
            if(e.who == '0')
            {
              chooseMessageAttackWho(in);
            }
            else
            {
              if(e.when == '0')
              {
                chooseMessageAttackWhoWhen(in);
              }
              else
              {
                if(e.route == '0')
                {
                  chooseMessageAttackWhoWhenRoute(in);
                }
                else
                {
                  chooseMessageAttackWhoWhenRouteConfirm(in);
                }
              }
            }
          }
          else if(e.what == 'd')
          {
            if(e.when == '0')
            {
              chooseMessageDefendWhen(in);
            }
            else
            {
              if(e.route == '0')
              {
                chooseMessageDefendWhenRoute(in);
              }
              else
              {
                chooseMessageDefendWhenRouteConfirm(in);
              }
            }
          }
        }
        else if(e.action == 's')
        {
          if(e.how == '0')
          {
            chooseSabotageHow(in);
          }
          else if(e.how == 'b')
          {
            chooseSabotageBlockConfirm(in);
          }
          else if(e.how == 'r')
          {
            chooseSabotageReadConfirm(in);
          }
          else if(e.how == 's')
          {
            if(e.which == '0')
            {
              chooseSabotageSwitchWhat(in);
            }
            else if(e.which == 'a')
            {
              if(e.what == '0')
              {
                chooseSabotageSwitchWhatWhat(in);
              }
              else if(e.what == 'a')
              {
                chooseSabotageSwitchWhatAttackConfirm(in);
              }
              else if(e.what == 'd')
              {
                chooseSabotageSwitchWhatDefendConfirm(in);
              }
            }
            else if(e.which == 'o')
            {
              if(e.who == '0')
              {
                chooseSabotageSwitchWhoWho(in);
              }
              else
              {
                chooseSabotageSwitchWhoWhoConfirm(in);
              }
            }
            else if(e.which == 'e')
            {
              if(e.when == '0')
              {
                chooseSabotageSwitchWhenWhen(in);
              }
              else
              {
                chooseSabotageSwitchWhenWhenConfirm(in);
              }
            }
          }
        }
      }
    }
    else if(s && reset_game_button.query(in))
    {
      c->requestReset();
    }
  }
}

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();

  for(int i = 0; i < 4; i++)
    cardImgs[i].tick(0.4f);

  if(anim_day < c->model.days)
  {
    anim_day += 0.01f;
    shown_day = anim_day;
  }
  if(anim_day > c->model.days) anim_day = (float)c->model.days;

  if(known_day != c->model.currentDay())
  {
    if(c->model.days == -1) return -1; //game was reset- go back to room

    zeroE();
    known_day = c->model.currentDay();
  }
  return 0;
}

float snapToInt(float v)
{
  float left = (float)((int)v);
  float right = (float)((int)(v+1.0f));
  if(v < left +0.01f) return left;
  if(v > right-0.01f) return right;
  return v;
}
void PlayScene::draw()
{
  float snapped_shown_day = snapToInt(shown_day);
  if(snapped_shown_day > c->model.days) snapped_shown_day = (float)c->model.days;
  if(snapped_shown_day < 0) snapped_shown_day = 0;
  int shown_prev_day = ((int)(snapped_shown_day+1.0f))-1; //round up, subtract 1
  float t = snapped_shown_day-((float)shown_prev_day);

  //draws cardinals and actions
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);

    cardImgs[i].draw(graphics);
    cardLbls[i].draw(graphics);

    if(t != 0.0f)
    {
      Event e = c->model.cardinalDayAction(card, shown_prev_day);
      if(e.action == 'a') graphics->draw(sword_s, rectForTraversal(card,e.who,t));
      if(e.action == 'd') graphics->draw(shield_full_s, rectForExpansion(card,t));
      if(e.action == 's') graphics->draw(red_x_s, rectForExpansion(card,t));
      if(e.action == 'm') graphics->draw(envelope_s, rectForTraversal(card,e.route,t));
      e = c->model.cardinalDayAction(card, shown_prev_day-1);
      if(e.action == 'm')
      {
        char at = e.route;
        char to = e.to;
        e = c->model.cardinalDayAction(at, shown_prev_day-1);
        if(!(e.action == 's' && e.how == 'b'))
          graphics->draw(envelope_s, rectForTraversal(at,to,t));
      }
    }
  }

  SDL_Rect sunr = rectForTransition(Week::day(shown_prev_day%7), Week::day((shown_prev_day+1)%7), t);
  graphics->draw(sun_s,sunr);
  sunBtn.rect.rect = sunr;
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(graphics);

  if(c->iWin())
  {
    drawWin();
  }
  else if(c->iLose())
  {
    drawLose();
  }
  else if(c->iTie())
  {
    drawTie();
  }
  else
  {
    Messenger m;
    Messenger m0;
    Messenger m1;
    if(c->myMessage(m))
    {
      drawMessage();
    }
    if(c->mySabotage(m0, m1))
    {
      if(m0.id) drawSabotage0();
      if(m1.id) drawSabotage1();
    }
    if(!c->iHaveAction())
    {
      if(e.action == '0')
      {
        drawWhat();
      }
      else if(e.action == 'a')
      {
        if(e.who == '0')
        {
          drawAttackWho();
        }
        else
        {
          drawAttackWhoConfirm();
        }
      }
      else if(e.action == 'd')
      {
        drawDefendConfirm();
      }
      else if(e.action == 'm')
      {
        if(e.what == '0')
        {
          drawMessageWhat();
        }
        else if(e.what == 'a')
        {
          if(e.who == '0')
          {
            drawMessageAttackWho();
          }
          else
          {
            if(e.when == '0')
            {
              drawMessageAttackWhoWhen();
            }
            else
            {
              if(e.route == '0')
              {
                drawMessageAttackWhoWhenRoute();
              }
              else
              {
                drawMessageAttackWhoWhenRouteConfirm();
              }
            }
          }
        }
        else if(e.what == 'd')
        {
          if(e.when == '0')
          {
            drawMessageDefendWhen();
          }
          else
          {
            if(e.route == '0')
            {
              drawMessageDefendWhenRoute();
            }
            else
            {
              drawMessageDefendWhenRouteConfirm();
            }
          }
        }
      }
      else if(e.action == 's')
      {
        if(e.how == '0')
        {
          drawSabotageHow();
        }
        else if(e.how == 'b')
        {
          drawSabotageBlockConfirm();
        }
        else if(e.how == 'r')
        {
          drawSabotageReadConfirm();
        }
        else if(e.how == 's')
        {
          if(e.which == '0')
          {
            drawSabotageSwitchWhat();
          }
          else if(e.which == 'a')
          {
            if(e.what == '0')
            {
              drawSabotageSwitchWhatWhat();
            }
            else if(e.what == 'a')
            {
              drawSabotageSwitchWhatAttackConfirm();
            }
            else if(e.what == 'd')
            {
              drawSabotageSwitchWhatDefendConfirm();
            }
          }
          else if(e.which == 'o')
          {
            if(e.who == '0')
            {
              drawSabotageSwitchWhoWho();
            }
            else
            {
              drawSabotageSwitchWhoWhoConfirm();
            }
          }
          else if(e.which == 'e')
          {
            if(e.when == '0')
            {
              drawSabotageSwitchWhenWhen();
            }
            else
            {
              drawSabotageSwitchWhenWhenConfirm();
            }
          }
        }
      }
    }
    else drawWaiting();
  }
}

void PlayScene::leave()
{
}
void PlayScene::pass()
{

}
void PlayScene::pop()
{
}

PlayScene::~PlayScene()
{

}

SDL_Rect PlayScene::rectForPosition(char c)
{
  return positionRects[Compass::icardinal(c)];
}

SDL_Rect PlayScene::rectForCardinal(char card)
{
  char me = c->myCardinal();
  while(me != 's')
  {
    me   = Compass::cwcardinal(me);
    card = Compass::cwcardinal(card);
  }
  return rectForPosition(card);
}

SDL_Rect PlayScene::rectForDay(char d)
{
  return dayRects[Week::iday(d)];
}

SDL_Rect PlayScene::rectForSun(char d)
{
  return sunRects[Week::iday(d)];
}

SDL_Rect PlayScene::rectForTraversal(char fcard, char tcard, float t)
{
  return LerpRect::lerp(rectForCardinal(fcard), rectForCardinal(tcard), t);
}

SDL_Rect PlayScene::rectForExpansion(char card, float t)
{
  SDL_Rect tmp = rectForCardinal(card);
  tmp.x -= tmp.w/2;
  tmp.y -= tmp.h/2;
  tmp.w *= 2;
  tmp.h *= 2;
  return LerpRect::lerp(rectForCardinal(card), tmp, t);
  return tmp;
}

SDL_Rect PlayScene::rectForTransition(char fd, char td, float t)
{
  return LerpRect::lerp(rectForSun(fd), rectForSun(td), t);
}

