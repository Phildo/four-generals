#ifndef _FG_SPRITE_H_
#define _FG_SPRITE_H_

#include "SDL.h"

namespace Sprite
{
  struct AnimSprites
  {
    SDL_Rect sprite[4];
    AnimSprites() {};
    AnimSprites(SDL_Rect s0, SDL_Rect s1, SDL_Rect s2, SDL_Rect s3)
    {
      sprite[0] = s0;
      sprite[1] = s1;
      sprite[2] = s2;
      sprite[3] = s3;
    }
  };

  void init(const char *f);

  extern SDL_Rect a[256];
  SDL_Rect alpha(char a);

  extern SDL_Rect arrow_left;
  extern SDL_Rect arrow_right;
  extern SDL_Rect arrow_left_up;
  extern SDL_Rect arrow_right_up;
  extern SDL_Rect undefined_1;
  extern SDL_Rect undefined_2;
  extern SDL_Rect undefined_3;
  extern SDL_Rect undefined_4;
  extern SDL_Rect question_bubble;
  extern SDL_Rect loading_0;
  extern SDL_Rect loading_1;
  extern SDL_Rect loading_2;
  extern SDL_Rect tally_1;
  extern SDL_Rect tally_2;
  extern SDL_Rect tally_3;
  extern SDL_Rect tally_4;
  extern SDL_Rect tally_5;

  extern SDL_Rect gen_n_0;
  extern SDL_Rect gen_n_1;
  extern SDL_Rect gen_n_2;
  extern SDL_Rect gen_n_3;
  extern SDL_Rect gen_e_0;
  extern SDL_Rect gen_e_1;
  extern SDL_Rect gen_e_2;
  extern SDL_Rect gen_e_3;
  extern SDL_Rect gen_s_0;
  extern SDL_Rect gen_s_1;
  extern SDL_Rect gen_s_2;
  extern SDL_Rect gen_s_3;
  extern SDL_Rect gen_w_0;
  extern SDL_Rect gen_w_1;
  extern SDL_Rect gen_w_2;
  extern SDL_Rect gen_w_3;

  extern SDL_Rect heart;
  extern SDL_Rect bolt;
  extern SDL_Rect bolt_empty;
  extern SDL_Rect shield;
  extern SDL_Rect sword;
  extern SDL_Rect knife;
  extern SDL_Rect envelope;
  extern SDL_Rect sun;
  extern SDL_Rect sblock;
  extern SDL_Rect sread;
  extern SDL_Rect sswitch;

  extern SDL_Rect square;
  extern SDL_Rect circle;
  extern SDL_Rect ex;
  extern SDL_Rect plus;

  extern SDL_Rect black_pixel;

  extern SDL_Rect border_top;
  extern SDL_Rect border_bottom;
  extern SDL_Rect border_left;
  extern SDL_Rect border_right;

  extern SDL_Rect border_corner_tl;
  extern SDL_Rect border_corner_tr;
  extern SDL_Rect border_corner_bl;
  extern SDL_Rect border_corner_br;

  extern SDL_Rect border_corner_round_tl;
  extern SDL_Rect border_corner_round_tr;
  extern SDL_Rect border_corner_round_bl;
  extern SDL_Rect border_corner_round_br;

  extern SDL_Rect p1;
  extern SDL_Rect p2;
  extern SDL_Rect p3;
  extern SDL_Rect p4;

  extern SDL_Rect p1_w;
  extern SDL_Rect p2_w;
  extern SDL_Rect p3_w;
  extern SDL_Rect p4_w;

  extern SDL_Rect p1_b;
  extern SDL_Rect p2_b;
  extern SDL_Rect p3_b;
  extern SDL_Rect p4_b;

  extern SDL_Rect p1_r;
  extern SDL_Rect p2_r;
  extern SDL_Rect p3_r;
  extern SDL_Rect p4_r;

  extern SDL_Rect what;
  extern SDL_Rect who_ns;
  extern SDL_Rect who_we;
  extern SDL_Rect when;
  extern SDL_Rect where_ns;
  extern SDL_Rect where_we;

  extern SDL_Rect null;

  extern SDL_Rect black;
  extern SDL_Rect white;
  extern SDL_Rect grey;
  extern SDL_Rect red;
  extern SDL_Rect green;
  extern SDL_Rect blue;

  //groupings
  extern SDL_Rect pTags[4];
  extern SDL_Rect pTagsW[4];
  extern SDL_Rect pTagsB[4];
  extern SDL_Rect pTagsR[4];

  extern AnimSprites general_anims[4];
  extern AnimSprites loading_anim;

};

#endif

