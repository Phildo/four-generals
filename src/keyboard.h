
struct Keyboard
{
  #define FG_MAX_KEYBOARD_Q_LEN 256
  circQ<char, FG_MAX_KEYBOARD_Q_LEN> in_q;

  char characters[50];
  Label glyphs[50];
  Button keys[50];

  Box rect;
  int keyw, keyh;

  Keyboard();
  Keyboard(SDL_Rect r);
  Keyboard(int x, int y, int w, int h);
  void touch(In &in);
  char poll();
  void draw(Graphics *g);

  //Ugh
  private:
    void initBoard();
    void initKeyInRow(SDL_Rect &r, int &i, char c);
    void enqueue(char c);
    char dequeue();
};

