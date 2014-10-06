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

bool mapPop = false;
SDL_Rect alphamap[52];
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
  }
  if(a >= 'a' && a <= 'z') return alphamap[a-'a'];
  if(a >= 'A' && a <= 'Z') return alphamap[a-'A'];
  return alphamap[0];
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
