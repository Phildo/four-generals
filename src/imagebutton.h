struct ImageButton
{
  SDL_Rect rect;
  SDL_Rect sprite;
  bool query(const In &in);
  void draw(Graphics *g);

  ImageButton();
  ImageButton(SDL_Rect r, SDL_Rect s);
  ImageButton(int x, int y, int w, int h, SDL_Rect s);
};

