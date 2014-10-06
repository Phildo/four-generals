#include <stdio.h>

int main()
{
  char id = 'a';
  int x = 0;
  int y = 100;
  int w = 18;
  int h = 31;
  for(int i = 0; i < 26; i++)
  {
    printf("SDL_Rect Sprite::alpha_%c() { return rect(%d,%d,%d,%d); }\n",id,x,y,w,h);
    id++;
    x += w;
  }
  id = 'A';
  x = 0;
  y += h;
  for(int i = 0; i < 26; i++)
  {
    printf("SDL_Rect Sprite::alpha_%c() { return rect(%d,%d,%d,%d); }\n",id,x,y,w,h);
    id++;
    x += w;
  }

}

