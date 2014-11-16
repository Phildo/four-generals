struct TextButton
{
  Box box;
  Label label;
  bool query(const In &in);
  void draw(Graphics *g);
  TextButton();
  TextButton(SDL_Rect r);
  TextButton(int x, int y, int w, int h);
  TextButton(SDL_Rect r, Label l);
  TextButton(int x, int y, int w, int h, Label l);
  TextButton(SDL_Rect r, const char *t);
  TextButton(int x, int y, int w, int h, const char *t);
  TextButton(SDL_Rect r, const char *t, int l);
  TextButton(int x, int y, int w, int h, const char *t, int l);
};

