struct Box
{
  union
  {
    SDL_Rect rect;
    struct
    {
      int x;
      int y;
      int w;
      int h;
    };
  };

  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);
  Box();
  Box(SDL_Rect r);
  Box(int x, int y, int w, int h);
};

