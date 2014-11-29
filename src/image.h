struct Image
{
  SDL_Rect rect;
  SDL_Rect sprite;
  void draw(Graphics *g);
  Image();
  Image(SDL_Rect s, SDL_Rect r);
  Image(SDL_Rect s, int x, int y, int w, int h);
};

