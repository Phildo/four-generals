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
  Box();
  Box(SDL_Rect r);
  Box(int x, int y, int w, int h);
};

