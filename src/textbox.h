struct TextBox
{
  #define FG_MAX_TEXTBOX_LEN 256
  int txt_len;
  char txt[FG_MAX_TEXTBOX_LEN+1]; //+1 for null

  Box rect; //really only uses its x,y,h
  static const int n_w = 18; //native_width in tex
  static const int n_h = 31; //native_height in tex
  void setText(String s);
  String getText();
  void input(char c);
  void backspace();
  void clear();
  void draw(Graphics *g);

  TextBox();
  TextBox(SDL_Rect r);
  TextBox(int x, int y, int w, int h);
};

