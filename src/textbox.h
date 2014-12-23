
struct TextBox
{
  #define FG_MAX_TEXTBOX_LEN 256
  int txt_len;
  char txt[FG_MAX_TEXTBOX_LEN+1]; //+1 for null
  Label label;

  Box box;

  TextBox();
  TextBox(SDL_Rect r);
  TextBox(int x, int y, int w, int h);
  void setText(String s);
  String getText();
  void input(char c);
  void backspace();
  void clear();
  void draw(Graphics *g);
};

