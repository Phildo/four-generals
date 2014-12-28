struct Label
{
  String text;
  Box rect; //really only uses its x,y,h
  static const int n_w = 18; //native_width in tex
  static const int n_h = 31; //native_height in tex
  void draw(Graphics *g);

  Label();
  Label(const char *t, SDL_Rect r);
  Label(const char *t, int l, SDL_Rect r);
  Label(const char *t, int l, int x, int y, int h);
  Label(const char *t, int x, int y, int h);
};

