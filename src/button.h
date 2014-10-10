struct Button
{
  SDL_Rect rect;
  bool query(const In &in);
  void draw(Graphics *g);
  Button();
  Button(SDL_Rect r);
  Button(int x, int y, int w, int h);
};
