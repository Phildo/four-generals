#include "sprite.h"
//#include "stdio.h"
#include "string.h"
#include "logger.h"

namespace Sprite
{
SDL_Rect a[256];

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

SDL_Rect heart;
SDL_Rect bolt;
SDL_Rect bolt_empty;
SDL_Rect shield;
SDL_Rect sword;
SDL_Rect knife;
SDL_Rect envelope;
SDL_Rect sun;
SDL_Rect telescope;

SDL_Rect square;
SDL_Rect circle;
SDL_Rect ex;
SDL_Rect plus;

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

SDL_Rect black;
SDL_Rect white;
SDL_Rect grey;
SDL_Rect red;
SDL_Rect green;
SDL_Rect blue;

SDL_Rect black_a;
SDL_Rect white_a;
SDL_Rect grey_a;
SDL_Rect red_a;
SDL_Rect green_a;
SDL_Rect blue_a;

SDL_Rect pTags[4];
SDL_Rect pTagsW[4];
SDL_Rect pTagsB[4];
SDL_Rect pTagsR[4];

SDL_Rect generals[4];
AnimSprites general_anims[4];
AnimSprites loading_anim;
};


static int parseIntoRect(char *b, SDL_Rect *v) //returns length traversed
{
  int i = 0;
  int j = 0;

  while(*(b+i) != ',') i++; *(b+i) = '\0';
  v->x = String(b+j).intVal();
  i++;
  j = i;

  while(*(b+i) != ',') i++; *(b+i) = '\0';
  v->y = String(b+j).intVal();
  i++;
  j = i;

  while(*(b+i) != ',') i++; *(b+i) = '\0';
  v->w = String(b+j).intVal();
  i++;
  j = i;

  while(*(b+i) != ',') i++; *(b+i) = '\0';
  v->h = String(b+j).intVal();
  i++;
  j = i;

  return j;
}

void Sprite::init(const char *f)
{
  String file_name(f);
  #ifdef FG_ANDROID
  //do nothing extra
  #elif defined FG_PC
  file_name = String("../assets/").concat(file_name);
  #endif

  const int n = 128; //num fields
  SDL_Rect *values[n];
  String keys[n];

  int i = 0;
  values[i] = &arrow_left;      keys[i] = String("arrow_left"); i++;
  values[i] = &arrow_right;     keys[i] = String("arrow_right"); i++;
  values[i] = &arrow_left_up;   keys[i] = String("arrow_left_up"); i++;
  values[i] = &arrow_right_up;  keys[i] = String("arrow_right_up"); i++;
  values[i] = &undefined_1;     keys[i] = String("undefined_1"); i++;
  values[i] = &undefined_2;     keys[i] = String("undefined_2"); i++;
  values[i] = &undefined_3;     keys[i] = String("undefined_3"); i++;
  values[i] = &undefined_4;     keys[i] = String("undefined_4"); i++;
  values[i] = &question_bubble; keys[i] = String("question_bubble"); i++;
  values[i] = &loading_0;       keys[i] = String("loading_0"); i++;
  values[i] = &loading_1;       keys[i] = String("loading_1"); i++;
  values[i] = &loading_2;       keys[i] = String("loading_2"); i++;
  values[i] = &tally_1;         keys[i] = String("tally_1"); i++;
  values[i] = &tally_2;         keys[i] = String("tally_2"); i++;
  values[i] = &tally_3;         keys[i] = String("tally_3"); i++;
  values[i] = &tally_4;         keys[i] = String("tally_4"); i++;
  values[i] = &tally_5;         keys[i] = String("tally_5"); i++;

  values[i] = &gen_n_0; keys[i] = String("gen_n_0"); i++;
  values[i] = &gen_n_1; keys[i] = String("gen_n_1"); i++;
  values[i] = &gen_n_2; keys[i] = String("gen_n_2"); i++;
  values[i] = &gen_n_3; keys[i] = String("gen_n_3"); i++;
  values[i] = &gen_e_0; keys[i] = String("gen_e_0"); i++;
  values[i] = &gen_e_1; keys[i] = String("gen_e_1"); i++;
  values[i] = &gen_e_2; keys[i] = String("gen_e_2"); i++;
  values[i] = &gen_e_3; keys[i] = String("gen_e_3"); i++;
  values[i] = &gen_s_0; keys[i] = String("gen_s_0"); i++;
  values[i] = &gen_s_1; keys[i] = String("gen_s_1"); i++;
  values[i] = &gen_s_2; keys[i] = String("gen_s_2"); i++;
  values[i] = &gen_s_3; keys[i] = String("gen_s_3"); i++;
  values[i] = &gen_w_0; keys[i] = String("gen_w_0"); i++;
  values[i] = &gen_w_1; keys[i] = String("gen_w_1"); i++;
  values[i] = &gen_w_2; keys[i] = String("gen_w_2"); i++;
  values[i] = &gen_w_3; keys[i] = String("gen_w_3"); i++;

  values[i] = &heart; keys[i] = String("heart"); i++;
  values[i] = &bolt; keys[i] = String("bolt"); i++;
  values[i] = &bolt_empty; keys[i] = String("bolt_empty"); i++;
  values[i] = &shield; keys[i] = String("shield"); i++;
  values[i] = &sword; keys[i] = String("sword"); i++;
  values[i] = &knife; keys[i] = String("knife"); i++;
  values[i] = &envelope; keys[i] = String("envelope"); i++;
  values[i] = &sun; keys[i] = String("sun"); i++;
  values[i] = &telescope; keys[i] = String("telescope"); i++;

  values[i] = &square; keys[i] = String("square"); i++;
  values[i] = &circle; keys[i] = String("circle"); i++;
  values[i] = &ex; keys[i] = String("ex"); i++;
  values[i] = &plus; keys[i] = String("plus"); i++;

  values[i] = &border_top;    keys[i] = String("border_top"); i++;
  values[i] = &border_bottom; keys[i] = String("border_bottom"); i++;
  values[i] = &border_left;   keys[i] = String("border_left"); i++;
  values[i] = &border_right;  keys[i] = String("border_right"); i++;

  values[i] = &border_corner_tl; keys[i] = String("border_corner_tl"); i++;
  values[i] = &border_corner_tr; keys[i] = String("border_corner_tr"); i++;
  values[i] = &border_corner_bl; keys[i] = String("border_corner_bl"); i++;
  values[i] = &border_corner_br; keys[i] = String("border_corner_br"); i++;

  values[i] = &border_corner_round_tl; keys[i] = String("border_corner_round_tl"); i++;
  values[i] = &border_corner_round_tr; keys[i] = String("border_corner_round_tr"); i++;
  values[i] = &border_corner_round_bl; keys[i] = String("border_corner_round_bl"); i++;
  values[i] = &border_corner_round_br; keys[i] = String("border_corner_round_br"); i++;

  values[i] = &p1; keys[i] = String("p1"); i++;
  values[i] = &p2; keys[i] = String("p2"); i++;
  values[i] = &p3; keys[i] = String("p3"); i++;
  values[i] = &p4; keys[i] = String("p4"); i++;

  values[i] = &p1_w; keys[i] = String("p1_w"); i++;
  values[i] = &p2_w; keys[i] = String("p2_w"); i++;
  values[i] = &p3_w; keys[i] = String("p3_w"); i++;
  values[i] = &p4_w; keys[i] = String("p4_w"); i++;

  values[i] = &p1_b; keys[i] = String("p1_b"); i++;
  values[i] = &p2_b; keys[i] = String("p2_b"); i++;
  values[i] = &p3_b; keys[i] = String("p3_b"); i++;
  values[i] = &p4_b; keys[i] = String("p4_b"); i++;

  values[i] = &p1_r; keys[i] = String("p1_r"); i++;
  values[i] = &p2_r; keys[i] = String("p2_r"); i++;
  values[i] = &p3_r; keys[i] = String("p3_r"); i++;
  values[i] = &p4_r; keys[i] = String("p4_r"); i++;

  values[i] = &what;     keys[i] = String("what"); i++;
  values[i] = &who_ns;   keys[i] = String("who_ns"); i++;
  values[i] = &who_we;   keys[i] = String("who_we"); i++;
  values[i] = &when;     keys[i] = String("when"); i++;
  values[i] = &where_ns; keys[i] = String("where_ns"); i++;
  values[i] = &where_we; keys[i] = String("where_we"); i++;

  values[i] = &null; keys[i] = String("null"); i++;

  values[i] = &black; keys[i] = String("black"); i++;
  values[i] = &white; keys[i] = String("white"); i++;
  values[i] = &grey; keys[i] = String("grey"); i++;
  values[i] = &red; keys[i] = String("red"); i++;
  values[i] = &green; keys[i] = String("green"); i++;
  values[i] = &blue; keys[i] = String("blue"); i++;

  values[i] = &black_a; keys[i] = String("black_a"); i++;
  values[i] = &white_a; keys[i] = String("white_a"); i++;
  values[i] = &grey_a; keys[i] = String("grey_a"); i++;
  values[i] = &red_a; keys[i] = String("red_a"); i++;
  values[i] = &green_a; keys[i] = String("green_a"); i++;
  values[i] = &blue_a; keys[i] = String("blue_a"); i++;

  String k;
  SDL_Rect *v;
  char buff[255]; //key+equals+4#s+4,s+buffer
  //FILE *fp = fopen(file_name.ptr(),"r");
  SDL_RWops *io = SDL_RWFromFile(file_name.ptr(), "r");
  //while(fgets(buff, sizeof(buff), (FILE*)fp))

  //first time
  int l = 0;
  while(io->read(io, buff+l, 1, 1) > 0 && buff[l] != '\n') l++;

  while(l)
  {
    int i = 0;
    int j = 0;
    while(buff[i] != '=') i++; buff[i] = '\0';
    k = String(buff);
    v = 0;
    if(k.len() > 1) //special
    {
      for(j = 0; j < n && !v; j++)
      {
        if(keys[j].equals(k))
        {
          v = values[j];
          parseIntoRect(&buff[i+1],v);
        }
      }
    }
    else //alphanum
    {
      char c = *k.ptr();
           if(c >= 'a' && c <= 'z') { c -= 'a'; }
      else if(c >= 'A' && c <= 'Z') { c -= 'A'; c += ('z'-'a')+1; }
      else if(c >= '0' && c <= '9') { c -= '0'; c += ('z'-'a')+1; c+= ('Z'-'A')+1; }
      else if(c == ' ') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+0;
      else if(c == '(') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+1;
      else if(c == '!') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+2;
      else if(c == '@') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+3;
      else if(c == '#') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+4;
      else if(c == '$') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+5;
      else if(c == '%') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+6;
      else if(c == '&') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+7;
      else if(c == '.') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+8;
      else if(c == ',') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+9;
      else if(c == '?') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+10;
      else if(c == ':') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+11;
      else if(c == ';') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+12;
      else if(c == ')') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+13;
      else if(c == '<') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+14;
      parseIntoRect(&buff[i+1],&a[(int)c]);
    }

    //hack for SDL_RWops
    l = 0;
    while(io->read(io, buff+l, 1, 1) > 0 && buff[l] != '\n') l++;
  }
  //fclose(fp);
  io->close(io);

  //construct groupings
  pTags[0]  = Sprite::p1;    pTags[1] = Sprite::p2;    pTags[2] = Sprite::p3;    pTags[3] = Sprite::p4;
  pTagsW[0] = Sprite::p1_w; pTagsW[1] = Sprite::p2_w; pTagsW[2] = Sprite::p3_w; pTagsW[3] = Sprite::p4_w;
  pTagsB[0] = Sprite::p1_b; pTagsB[1] = Sprite::p2_b; pTagsB[2] = Sprite::p3_b; pTagsB[3] = Sprite::p4_b;
  pTagsR[0] = Sprite::p1_r; pTagsR[1] = Sprite::p2_r; pTagsR[2] = Sprite::p3_r; pTagsR[3] = Sprite::p4_r;

  generals[0] = Sprite::gen_n_0;
  generals[1] = Sprite::gen_e_0;
  generals[2] = Sprite::gen_s_0;
  generals[3] = Sprite::gen_w_0;

  general_anims[0] = AnimSprites(Sprite::gen_n_0, Sprite::gen_n_1, Sprite::gen_n_2, Sprite::gen_n_3);
  general_anims[1] = AnimSprites(Sprite::gen_e_0, Sprite::gen_e_1, Sprite::gen_e_2, Sprite::gen_e_3);
  general_anims[2] = AnimSprites(Sprite::gen_s_0, Sprite::gen_s_1, Sprite::gen_s_2, Sprite::gen_s_3);
  general_anims[3] = AnimSprites(Sprite::gen_w_0, Sprite::gen_w_1, Sprite::gen_w_2, Sprite::gen_w_3);

  loading_anim = AnimSprites(Sprite::loading_0, Sprite::loading_1, Sprite::loading_2, Sprite::loading_1);
}

SDL_Rect Sprite::alpha(char c)
{
       if(c >= 'a' && c <= 'z') { c -= 'a'; }
  else if(c >= 'A' && c <= 'Z') { c -= 'A'; c += ('z'-'a')+1; }
  else if(c >= '0' && c <= '9') { c -= '0'; c += ('z'-'a')+1; c+= ('Z'-'A')+1; }
  else if(c == ' ') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+0;
  else if(c == '(') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+1;
  else if(c == '!') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+2;
  else if(c == '@') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+3;
  else if(c == '#') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+4;
  else if(c == '$') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+5;
  else if(c == '%') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+6;
  else if(c == '&') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+7;
  else if(c == '.') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+8;
  else if(c == ',') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+9;
  else if(c == '?') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+10;
  else if(c == ':') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+11;
  else if(c == ';') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+12;
  else if(c == ')') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+13;
  else if(c == '<') c = ('z'-'a')+('Z'-'A')+('9'-'0')+3+14;
  return a[(int)c];
}

