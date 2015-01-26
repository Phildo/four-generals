struct ImageButtonRound
{
  SDL_Rect sprite;
  Box rect;
  bool query(const In &in);
  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);

  ImageButtonRound();
  ImageButtonRound(SDL_Rect s, SDL_Rect r);
  ImageButtonRound(SDL_Rect s, int x, int y, int w, int h);
};

