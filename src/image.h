struct Image
{
  SDL_Rect sprite;
  Box rect;
  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);
  Image();
  Image(SDL_Rect s, SDL_Rect r);
  Image(SDL_Rect s, int x, int y, int w, int h);
};

