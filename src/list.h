struct List
{
  SDL_Rect rect;
  int text_height;
  int num_rows; //derivable on fly, but whatevs

  static const int max_rows = 256;
  Label rows[max_rows];

  void draw(Graphics *g);
  void append(const char *b);
  List();
  //from rect.h + text height we will derive num rows
  List(SDL_Rect r, int t_h);
  List(int x, int y, int h, int w, int t_h);
};

