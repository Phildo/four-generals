struct Box
{
  SDL_Rect rect;
  void draw(Graphics *g);
  Box();
  Box(SDL_Rect r);
  Box(int x, int y, int w, int h);
};

