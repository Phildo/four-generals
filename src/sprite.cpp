#include "sprite.h"

SDL_Rect rect(int x, int y, int w, int h) { SDL_Rect r; r.x = x; r.y = y; r.w = w; r.h = h; return r; }
SDL_Rect Sprite::border_tl() { return rect(100, 0,10,10); }
SDL_Rect Sprite::border_t()  { return rect(110, 0,10,10); }
SDL_Rect Sprite::border_tr() { return rect(120, 0,10,10); }
SDL_Rect Sprite::border_l()  { return rect(100,10,10,10); }
SDL_Rect Sprite::border_r()  { return rect(120,10,10,10); }
SDL_Rect Sprite::border_bl() { return rect(100,20,10,10); }
SDL_Rect Sprite::border_b()  { return rect(110,20,10,10); }
SDL_Rect Sprite::border_br() { return rect(120,20,10,10); }

SDL_Rect Sprite::messenger() { return rect(130,0,10,10); }

bool mapPop = false;
SDL_Rect alphamap[78];
SDL_Rect Sprite::alpha(char a)
{
  if(!mapPop)
  {
    alphamap[0] = alpha_a();
    alphamap[1] = alpha_b();
    alphamap[2] = alpha_c();
    alphamap[3] = alpha_d();
    alphamap[4] = alpha_e();
    alphamap[5] = alpha_f();
    alphamap[6] = alpha_g();
    alphamap[7] = alpha_h();
    alphamap[8] = alpha_i();
    alphamap[9] = alpha_j();
    alphamap[10] = alpha_k();
    alphamap[11] = alpha_l();
    alphamap[12] = alpha_m();
    alphamap[13] = alpha_n();
    alphamap[14] = alpha_o();
    alphamap[15] = alpha_p();
    alphamap[16] = alpha_q();
    alphamap[17] = alpha_r();
    alphamap[18] = alpha_s();
    alphamap[19] = alpha_t();
    alphamap[20] = alpha_u();
    alphamap[21] = alpha_v();
    alphamap[22] = alpha_w();
    alphamap[23] = alpha_x();
    alphamap[24] = alpha_y();
    alphamap[25] = alpha_z();
    alphamap[26] = alpha_A();
    alphamap[27] = alpha_B();
    alphamap[28] = alpha_C();
    alphamap[29] = alpha_D();
    alphamap[30] = alpha_E();
    alphamap[31] = alpha_F();
    alphamap[32] = alpha_G();
    alphamap[33] = alpha_H();
    alphamap[34] = alpha_I();
    alphamap[35] = alpha_J();
    alphamap[36] = alpha_K();
    alphamap[37] = alpha_L();
    alphamap[38] = alpha_M();
    alphamap[39] = alpha_N();
    alphamap[40] = alpha_O();
    alphamap[41] = alpha_P();
    alphamap[42] = alpha_Q();
    alphamap[43] = alpha_R();
    alphamap[44] = alpha_S();
    alphamap[45] = alpha_T();
    alphamap[46] = alpha_U();
    alphamap[47] = alpha_V();
    alphamap[48] = alpha_W();
    alphamap[49] = alpha_X();
    alphamap[50] = alpha_Y();
    alphamap[51] = alpha_Z();
    alphamap[52] = alpha_0();
    alphamap[53] = alpha_1();
    alphamap[54] = alpha_2();
    alphamap[55] = alpha_3();
    alphamap[56] = alpha_4();
    alphamap[57] = alpha_5();
    alphamap[58] = alpha_6();
    alphamap[59] = alpha_7();
    alphamap[60] = alpha_8();
    alphamap[61] = alpha_9();
    alphamap[62] = alpha_space();
    alphamap[63] = alpha_paren_l();
    alphamap[64] = alpha_bang();
    alphamap[65] = alpha_at();
    alphamap[66] = alpha_sha();
    alphamap[67] = alpha_dollar();
    alphamap[68] = alpha_mod();
    alphamap[69] = alpha_amp();
    alphamap[70] = alpha_period();
    alphamap[71] = alpha_comma();
    alphamap[72] = alpha_question();
    alphamap[73] = alpha_colon();
    alphamap[74] = alpha_colon_semi();
    alphamap[75] = alpha_paren_r();
    alphamap[76] = alpha_back();
    alphamap[77] = alpha_NOPE();
  }
  if(a >= 'a' && a <= 'z') return alphamap[a-'a'];
  if(a >= 'A' && a <= 'Z') return alphamap[a-'A'];
  if(a == '0') return alphamap[52];
  if(a == '1') return alphamap[53];
  if(a == '2') return alphamap[54];
  if(a == '3') return alphamap[55];
  if(a == '4') return alphamap[56];
  if(a == '5') return alphamap[57];
  if(a == '6') return alphamap[58];
  if(a == '7') return alphamap[59];
  if(a == '8') return alphamap[60];
  if(a == '9') return alphamap[61];
  if(a == ' ') return alphamap[62];
  if(a == '(') return alphamap[63];
  if(a == '!') return alphamap[64];
  if(a == '@') return alphamap[65];
  if(a == '#') return alphamap[66];
  if(a == '$') return alphamap[67];
  if(a == '%') return alphamap[68];
  if(a == '&') return alphamap[69];
  if(a == '.') return alphamap[70];
  if(a == ',') return alphamap[71];
  if(a == '?') return alphamap[72];
  if(a == ':') return alphamap[73];
  if(a == ';') return alphamap[74];
  if(a == ')') return alphamap[75];
  if(a == '<') return alphamap[76];
  return alphamap[77];
}
SDL_Rect Sprite::alpha_a() { return rect(0,100,18,31); }
SDL_Rect Sprite::alpha_b() { return rect(18,100,18,31); }
SDL_Rect Sprite::alpha_c() { return rect(36,100,18,31); }
SDL_Rect Sprite::alpha_d() { return rect(54,100,18,31); }
SDL_Rect Sprite::alpha_e() { return rect(72,100,18,31); }
SDL_Rect Sprite::alpha_f() { return rect(90,100,18,31); }
SDL_Rect Sprite::alpha_g() { return rect(108,100,18,31); }
SDL_Rect Sprite::alpha_h() { return rect(126,100,18,31); }
SDL_Rect Sprite::alpha_i() { return rect(144,100,18,31); }
SDL_Rect Sprite::alpha_j() { return rect(162,100,18,31); }
SDL_Rect Sprite::alpha_k() { return rect(180,100,18,31); }
SDL_Rect Sprite::alpha_l() { return rect(198,100,18,31); }
SDL_Rect Sprite::alpha_m() { return rect(216,100,18,31); }
SDL_Rect Sprite::alpha_n() { return rect(234,100,18,31); }
SDL_Rect Sprite::alpha_o() { return rect(252,100,18,31); }
SDL_Rect Sprite::alpha_p() { return rect(270,100,18,31); }
SDL_Rect Sprite::alpha_q() { return rect(288,100,18,31); }
SDL_Rect Sprite::alpha_r() { return rect(306,100,18,31); }
SDL_Rect Sprite::alpha_s() { return rect(324,100,18,31); }
SDL_Rect Sprite::alpha_t() { return rect(342,100,18,31); }
SDL_Rect Sprite::alpha_u() { return rect(360,100,18,31); }
SDL_Rect Sprite::alpha_v() { return rect(378,100,18,31); }
SDL_Rect Sprite::alpha_w() { return rect(396,100,18,31); }
SDL_Rect Sprite::alpha_x() { return rect(414,100,18,31); }
SDL_Rect Sprite::alpha_y() { return rect(432,100,18,31); }
SDL_Rect Sprite::alpha_z() { return rect(450,100,18,31); }
SDL_Rect Sprite::alpha_A() { return rect(0,131,18,31); }
SDL_Rect Sprite::alpha_B() { return rect(18,131,18,31); }
SDL_Rect Sprite::alpha_C() { return rect(36,131,18,31); }
SDL_Rect Sprite::alpha_D() { return rect(54,131,18,31); }
SDL_Rect Sprite::alpha_E() { return rect(72,131,18,31); }
SDL_Rect Sprite::alpha_F() { return rect(90,131,18,31); }
SDL_Rect Sprite::alpha_G() { return rect(108,131,18,31); }
SDL_Rect Sprite::alpha_H() { return rect(126,131,18,31); }
SDL_Rect Sprite::alpha_I() { return rect(144,131,18,31); }
SDL_Rect Sprite::alpha_J() { return rect(162,131,18,31); }
SDL_Rect Sprite::alpha_K() { return rect(180,131,18,31); }
SDL_Rect Sprite::alpha_L() { return rect(198,131,18,31); }
SDL_Rect Sprite::alpha_M() { return rect(216,131,18,31); }
SDL_Rect Sprite::alpha_N() { return rect(234,131,18,31); }
SDL_Rect Sprite::alpha_O() { return rect(252,131,18,31); }
SDL_Rect Sprite::alpha_P() { return rect(270,131,18,31); }
SDL_Rect Sprite::alpha_Q() { return rect(288,131,18,31); }
SDL_Rect Sprite::alpha_R() { return rect(306,131,18,31); }
SDL_Rect Sprite::alpha_S() { return rect(324,131,18,31); }
SDL_Rect Sprite::alpha_T() { return rect(342,131,18,31); }
SDL_Rect Sprite::alpha_U() { return rect(360,131,18,31); }
SDL_Rect Sprite::alpha_V() { return rect(378,131,18,31); }
SDL_Rect Sprite::alpha_W() { return rect(396,131,18,31); }
SDL_Rect Sprite::alpha_X() { return rect(414,131,18,31); }
SDL_Rect Sprite::alpha_Y() { return rect(432,131,18,31); }
SDL_Rect Sprite::alpha_Z() { return rect(450,131,18,31); }
SDL_Rect Sprite::alpha_0() { return rect(0,162,18,31); }
SDL_Rect Sprite::alpha_1() { return rect(18,162,18,31); }
SDL_Rect Sprite::alpha_2() { return rect(36,162,18,31); }
SDL_Rect Sprite::alpha_3() { return rect(54,162,18,31); }
SDL_Rect Sprite::alpha_4() { return rect(72,162,18,31); }
SDL_Rect Sprite::alpha_5() { return rect(90,162,18,31); }
SDL_Rect Sprite::alpha_6() { return rect(108,162,18,31); }
SDL_Rect Sprite::alpha_7() { return rect(126,162,18,31); }
SDL_Rect Sprite::alpha_8() { return rect(144,162,18,31); }
SDL_Rect Sprite::alpha_9() { return rect(162,162,18,31); }
SDL_Rect Sprite::alpha_space() { return rect(180,162,18,31); }
SDL_Rect Sprite::alpha_paren_l() { return rect(198,162,18,31); }
SDL_Rect Sprite::alpha_bang() { return rect(216,162,18,31); }
SDL_Rect Sprite::alpha_at() { return rect(234,162,18,31); }
SDL_Rect Sprite::alpha_sha() { return rect(252,162,18,31); }
SDL_Rect Sprite::alpha_dollar() { return rect(270,162,18,31); }
SDL_Rect Sprite::alpha_mod() { return rect(288,162,18,31); }
SDL_Rect Sprite::alpha_amp() { return rect(306,162,18,31); }
SDL_Rect Sprite::alpha_period() { return rect(324,162,18,31); }
SDL_Rect Sprite::alpha_comma() { return rect(342,162,18,31); }
SDL_Rect Sprite::alpha_question() { return rect(360,162,18,31); }
SDL_Rect Sprite::alpha_colon() { return rect(378,162,18,31); }
SDL_Rect Sprite::alpha_colon_semi() { return rect(396,162,18,31); }
SDL_Rect Sprite::alpha_paren_r() { return rect(414,162,18,31); }
SDL_Rect Sprite::alpha_back() { return rect(432,162,18,31); }
SDL_Rect Sprite::alpha_NOPE() { return rect(450,162,18,31); }

