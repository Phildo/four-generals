
struct Textbox
{
  #define MAX_TEXTBOX_LEN 256
  int textLen;
  char text[MAX_TEXTBOX_LEN+1]; //+1 for null
  Label string;

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

