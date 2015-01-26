struct List
{
  int text_height;
  Box rect;

  static const int max_rows = 256;
  int num_rows; //derivable on fly, but whatevs

  Label rows[max_rows];

  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);
  void append(const char *b);
  List();
  //from rect.h + text height we will derive num rows
  List(SDL_Rect r, int t_h);
  List(int x, int y, int h, int w, int t_h);
};

