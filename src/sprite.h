#ifndef _FG_SPRITE_H_
#define _FG_SPRITE_H_

#include "SDL.h"

namespace Sprite
{
  void init(const char *f);

  SDL_Rect alpha[256];
  SDL_Rect alpha(char a);

  SDL_Rect arrow_left;
  SDL_Rect arrow_right;
  SDL_Rect arrow_left_up;
  SDL_Rect arrow_right_up;
  SDL_Rect undefined_1;
  SDL_Rect undefined_2;
  SDL_Rect undefined_3;
  SDL_Rect undefined_4;
  SDL_Rect question_bubble;
  SDL_Rect loading_0;
  SDL_Rect loading_1;
  SDL_Rect loading_2;
  SDL_Rect tally_1;
  SDL_Rect tally_2;
  SDL_Rect tally_3;
  SDL_Rect tally_4;
  SDL_Rect tally_5;

  SDL_Rect gen_n_0;
  SDL_Rect gen_n_1;
  SDL_Rect gen_n_2;
  SDL_Rect gen_n_3;
  SDL_Rect gen_e_0;
  SDL_Rect gen_e_1;
  SDL_Rect gen_e_2;
  SDL_Rect gen_e_3;
  SDL_Rect gen_s_0;
  SDL_Rect gen_s_1;
  SDL_Rect gen_s_2;
  SDL_Rect gen_s_3;
  SDL_Rect gen_w_0;
  SDL_Rect gen_w_1;
  SDL_Rect gen_w_2;
  SDL_Rect gen_w_3;

  SDL_Rect shield;
  SDL_Rect sword;
  SDL_Rect red_x;
  SDL_Rect envelope;
  SDL_Rect sun;
  SDL_Rect sblock;
  SDL_Rect sread;
  SDL_Rect sswitch;

  SDL_Rect black_pixel;

  SDL_Rect border_top;
  SDL_Rect border_bottom;
  SDL_Rect border_left;
  SDL_Rect border_right;

  SDL_Rect border_corner_tl;
  SDL_Rect border_corner_tr;
  SDL_Rect border_corner_bl;
  SDL_Rect border_corner_br;

  SDL_Rect border_corner_round_tl;
  SDL_Rect border_corner_round_tr;
  SDL_Rect border_corner_round_bl;
  SDL_Rect border_corner_round_br;

  SDL_Rect p1;
  SDL_Rect p2;
  SDL_Rect p3;
  SDL_Rect p4;

  SDL_Rect p1_w;
  SDL_Rect p2_w;
  SDL_Rect p3_w;
  SDL_Rect p4_w;

  SDL_Rect p1_b;
  SDL_Rect p2_b;
  SDL_Rect p3_b;
  SDL_Rect p4_b;

  SDL_Rect p1_r;
  SDL_Rect p2_r;
  SDL_Rect p3_r;
  SDL_Rect p4_r;

  SDL_Rect what;
  SDL_Rect who_ns;
  SDL_Rect who_we;
  SDL_Rect when;
  SDL_Rect where_ns;
  SDL_Rect where_we;

  SDL_Rect null;
};

#endif

