struct TextButton
{
  String text;
  Box rect; //really only uses its x,y,h
  static const int n_w = 18; //native_width in tex
  static const int n_h = 31; //native_height in tex
  bool query(const In &in);
  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);

  TextButton();
  TextButton(const char *t, SDL_Rect r);
  TextButton(const char *t, int l, SDL_Rect r);
  TextButton(const char *t, int l, int x, int y, int w, int h);
  TextButton(const char *t, int x, int y, int w, int h);
};

