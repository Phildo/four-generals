#include <stdio.h>

int writeNumToBuff(char *b, int i, int n)
{
  int d = 0;
      if(n > 999) d = 4;
  else if(n > 99) d = 3;
   else if(n > 9) d = 2;
    else          d = 1;

  int old_i = i;
  i += (d-1);

  int digit = 0;
  while(i >= old_i)
  {
    digit = n%10;
    b[i] = '0'+digit;
    n /= 10;
    i--;
  }
  return old_i + d;
}

void right(int &x, int &y, int &w, int &h)
{
  x+=w;
}

void down(int &x, int &y, int &w, int &h)
{
  y+=h;
  x=0;
}

void rect(char *b, int i, int x, int y, int w, int h)
{
  b[i++] = '=';
  i = writeNumToBuff(b, i, x);
  b[i++] = ',';
  i = writeNumToBuff(b, i, y);
  b[i++] = ',';
  i = writeNumToBuff(b, i, w);
  b[i++] = ',';
  i = writeNumToBuff(b, i, h);
  b[i++] = ',';
  b[i++] = '\n';
  b[i] = '\0';
}

void sprite(char *b, const char *s, int x, int y, int w, int h)
{
  int i = 0;
  while(*(s+i) != '\0')
  {
    *(b+i) = *(s+i);
    i++;
  }
  rect(b, i, x,y,w,h);
}

int main(int argc, char* argv[])
{
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  char buff[255];
  FILE *fp = fopen("sprite.txt","w+");

  x = 0;
  y = 0;
  w = 18;
  h = 31;
  char alphabuff[2]; alphabuff[1] = '\0';
  for(alphabuff[0] = 'a'; alphabuff[0] <= 'z'; alphabuff[0]++)
  {
    sprite(buff,alphabuff,x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  }
  down(x,y,w,h);
  for(alphabuff[0] = 'A'; alphabuff[0] <= 'Z'; alphabuff[0]++)
  {
    sprite(buff,alphabuff,x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  }
  down(x,y,w,h);
  for(alphabuff[0] = '0'; alphabuff[0] <= '9'; alphabuff[0]++)
  {
    sprite(buff,alphabuff,x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  }

  sprite(buff," ",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"(",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"!",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"@",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"#",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"$",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"%",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"&",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,".",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,",",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"?",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,":",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,";",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,")",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"<",x,y,w,h); fputs(buff,fp); 

  x = 0;
  y = 100;
  w = 32;
  h = 32;
  sprite(buff,"arrow_left",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"arrow_right",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"undefined_1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"undefined_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"question_bubble",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"loading_0",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"tally_1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"tally_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"tally_5",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"arrow_left_up",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"arrow_right_up",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"undefined_3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"undefined_4",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"loading_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"loading_1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"tally_3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"tally_4",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"gen_n_0",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_n_1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_e_0",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_e_1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_s_0",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_s_1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_w_0",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_w_1",x,y,w,h); fputs(buff,fp); 

  down(x,y,w,h);

  sprite(buff,"gen_n_3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_n_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_e_3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_e_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_s_3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_s_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_w_3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"gen_w_2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"heart",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"bolt",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"bolt_empty",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"shield",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"sword",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"knife",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"envelope",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"sun",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"sblock",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"sread",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"sswitch",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"square",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"circle",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"ex",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"plus",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"p1",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p2",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p1_w",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p2_w",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p1_b",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p2_b",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p1_r",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p2_r",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"p3",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p4",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p3_w",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p4_w",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p3_b",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p4_b",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p3_r",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"p4_r",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"what",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"who_ns",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"who_we",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"when",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"where_ns",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"where_we",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);
  sprite(buff,"null",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  //special
  x = 0;
  y = 100;
  w = 32;
  h = 32;
  down(x,y,w,h);
  down(x,y,w,h);
  down(x,y,w,h);
  down(x,y,w,h);
  down(x,y,w,h);

  int bordery = y;
  //get to borders
  x += 8; //offset
  w = 16;
  h = 16;
  sprite(buff,"border_top",x,y,w,h); fputs(buff,fp); down(x,y,w,h);
  x += 8;
  sprite(buff,"border_bottom",x,y,w,h); fputs(buff,fp); 
  x = 0;
  y = bordery+8;
  sprite(buff,"border_left",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_right",x,y,w,h); fputs(buff,fp); 
  x = 0;
  y = bordery;
  sprite(buff,"border_corner_tl",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_corner_tr",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_corner_round_tl",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_corner_round_tr",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  down(x,y,w,h);

  sprite(buff,"border_corner_bl",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_corner_br",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_corner_round_bl",x,y,w,h); fputs(buff,fp); right(x,y,w,h);
  sprite(buff,"border_corner_round_br",x,y,w,h); fputs(buff,fp); right(x,y,w,h);

  fclose(fp);
}

