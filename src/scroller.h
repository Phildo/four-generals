struct Scroller
{
  Box rect;
  Box content_rect;
  Button button;

  bool down;
  int down_time;
  int last_y;
  int offset;

  void touch(const In &in);
  void tick();
  void draw(Graphics *g);

  float p();

  Scroller();
  Scroller(SDL_Rect r, SDL_Rect c);
  Scroller(int rx, int ry, int rh, int rw, int cx, int cy, int ch, int cw);
};

