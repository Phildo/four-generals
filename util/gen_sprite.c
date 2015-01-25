#include <stdio.h>

int writeNumToBuff(char *b, int i, int n)
{
  int d = 0;
      if(n > 999) d = 4;
  else if(n > 99) d = 3;
   else if(n > 9) d = 2;
    else          d = 1;

  int old_i = i;
  i += d;

  int digit = 0;
  while(i > old_i)
  {
    digit = n%10;
    b[i] = '0'+digit;
    n /= 10;
  }
  return old_i + d;
}

int main(int argc, char* argv[])
{
  int x;
  int y;
  int w;
  int h;
  char buff[255]; //key+equals+4#s+4,s+buffer
  FILE *fp = fopen(file_name.ptr(),"w+");
  fputs(buff,fp);
  fclose(fp);
}

