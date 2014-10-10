
struct Keyboard
{
  #define MAX_KEYBOARD_Q_LEN 256
  int inQLen;
  char inputQ[MAX_KEYBOARD_Q_LEN];

  char characters[50];
  Label glyphs[50];
  Button keys[50];

  int keyw, keyh;
  SDL_Rect rect;

  Keyboard();
  Keyboard(SDL_Rect r);
  Keyboard(int x, int y, int w, int h);
  void touch(In &in);
  char poll();
  void clear();
  void draw(Graphics *g);

  //Ugh
  private:
    void initBoard();
    void initKeyInRow(SDL_Rect &r, int &i, char c);
    void enqueue(char c);
    char dequeue();
};

