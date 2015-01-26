struct ImageButton
{
  SDL_Rect sprite;
  Box rect;
  bool query(const In &in);
  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);

  ImageButton();
  ImageButton(SDL_Rect s, SDL_Rect r);
  ImageButton(SDL_Rect s, int x, int y, int w, int h);
};

