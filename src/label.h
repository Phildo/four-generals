struct Label
{
  SDL_Rect rect; //really only uses its x,y
  char *string;
  int length;
  static const int n_w = 18; //native_width
  static const int n_h = 31; //native_height
  void draw(Graphics *g);
  Label();
  Label(SDL_Rect r, char *s, int l);
  Label(int x, int y, int h, char *s, int l);
};

