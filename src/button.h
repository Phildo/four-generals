struct Button
{
  Box rect;
  bool query(const In &in);
  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);
  Button();
  Button(SDL_Rect r);
  Button(int x, int y, int w, int h);
};

