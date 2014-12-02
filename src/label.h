struct Label
{
  SDL_Rect rect; //really only uses its x,y,h
  String text;
  static const int n_w = 18; //native_width
  static const int n_h = 31; //native_height
  void draw(Graphics *g);
  Label();
  Label(SDL_Rect r, const char *t, int l);
  Label(SDL_Rect r, const char *t);
  Label(int x, int y, int h, const char *t, int l);
  Label(int x, int y, int h, const char *t);
};

