#include "sprite.h"
#include "stdio.h"
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


static void parseIntoRect(char *b, SDL_Rect *v)
{
  fg_log("parsing %s",b);
  int i = 0;
  int j = 0;

  j = i;
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
  fg_log("into %d, %d, %d, %d",v->x,v->y,v->w,v->h);
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

  values[i] = &shield;   keys[i] = String("shield"); i++;
  values[i] = &sword;    keys[i] = String("sword"); i++;
  values[i] = &red_x;    keys[i] = String("red_x"); i++;
  values[i] = &envelope; keys[i] = String("envelope"); i++;
  values[i] = &sun;      keys[i] = String("sun"); i++;
  values[i] = &sblock;   keys[i] = String("sblock"); i++;
  values[i] = &sread;    keys[i] = String("sread"); i++;
  values[i] = &sswitch;  keys[i] = String("sswitch"); i++;

  values[i] = &black_pixel; keys[i] = String("black_pixel"); i++;

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

  String k;
  SDL_Rect *v;
  char buff[255]; //key+equals+4#s+4,s+buffer
  FILE *fp = fopen(file_name.ptr(),"r");
  while(fgets(buff, 255, (FILE*)fp))
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
  }
  fclose(fp);
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

