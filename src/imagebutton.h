struct ImageButton
{
  SDL_Rect sprite;
  Box rect;
  bool query(const In &in);
  void draw(Graphics *g);

  ImageButton();
  ImageButton(SDL_Rect s, SDL_Rect r);
  ImageButton(SDL_Rect s, int x, int y, int w, int h);
};

