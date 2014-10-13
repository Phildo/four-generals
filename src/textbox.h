
struct Textbox
{
  #define FG_MAX_TEXTBOX_LEN 256
  int textLen;
  char text[FG_MAX_TEXTBOX_LEN+1]; //+1 for null
  Label label;

  SDL_Rect rect;

  Textbox();
  Textbox(SDL_Rect r);
  Textbox(int x, int y, int w, int h);
  void getString(char *&c);
  void input(char c);
  void backspace();
  void clear();
  void draw(Graphics *g);
};

