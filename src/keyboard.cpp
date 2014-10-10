Keyboard::Keyboard() {}
Keyboard::Keyboard(int x, int y, int w, int h)
{
  inQLen = 0;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  keyw = rect.w/10;
  keyh = rect.h/4;
  initBoard();
}
Keyboard::Keyboard(SDL_Rect r)
{
  inQLen = 0;
  rect = r;
  keyw = rect.w/10;
  keyh = rect.h/4;
  initBoard();
}

void Keyboard::initBoard()
{
  inQLen = 0;

  SDL_Rect tmp;
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.w = keyw;
  tmp.h = keyh;

  int i = 0;
  char c = '1';
  for(int j = 1; j < 10; j++, c++) //1-9
    initKeyInRow(tmp, i, c);
  initKeyInRow(tmp, i, '0'); //0

  tmp.x = rect.x;
  tmp.y = rect.y+keyh;
  initKeyInRow(tmp, i, 'q');
  initKeyInRow(tmp, i, 'w');
  initKeyInRow(tmp, i, 'e');
  initKeyInRow(tmp, i, 'r');
  initKeyInRow(tmp, i, 't');
  initKeyInRow(tmp, i, 'y');
  initKeyInRow(tmp, i, 'u');
  initKeyInRow(tmp, i, 'i');
  initKeyInRow(tmp, i, 'o');
  initKeyInRow(tmp, i, 'p');

  tmp.x = rect.x;
  tmp.y = rect.y+keyh+keyh;
  initKeyInRow(tmp, i, 'a');
  initKeyInRow(tmp, i, 's');
  initKeyInRow(tmp, i, 'd');
  initKeyInRow(tmp, i, 'f');
  initKeyInRow(tmp, i, 'g');
  initKeyInRow(tmp, i, 'h');
  initKeyInRow(tmp, i, 'j');
  initKeyInRow(tmp, i, 'k');
  initKeyInRow(tmp, i, 'l');

  tmp.x = rect.x;
  tmp.y = rect.y+keyh+keyh+keyh;
  initKeyInRow(tmp, i, 'z');
  initKeyInRow(tmp, i, 'x');
  initKeyInRow(tmp, i, 'c');
  initKeyInRow(tmp, i, 'v');
  initKeyInRow(tmp, i, 'b');
  initKeyInRow(tmp, i, 'n');
  initKeyInRow(tmp, i, 'm');
  initKeyInRow(tmp, i, '.');
  initKeyInRow(tmp, i, '<');
}

void Keyboard::initKeyInRow(SDL_Rect &r, int &i, char c)
{
  characters[i] = c;
  glyphs[i] = Label(r, &characters[i], 1);
  keys[i] = Button(r);
  i++;
  r.x += r.w;
}

void Keyboard::touch(In &in)
{
  for(int i = 0; i < 38; i++)
    if(keys[i].query(in)) enqueue(characters[i]);
}

char Keyboard::poll()
{
  if(inQLen > 0)
    return dequeue();
  return 0;
}

void Keyboard::enqueue(char c)
{
  if(inQLen >= MAX_KEYBOARD_Q_LEN) return; //ignore input
  inputQ[inQLen] = c;
  inQLen++;
}

char Keyboard::dequeue()
{
  char c = inputQ[0];
  for(int i = 0; i < inQLen; i++)
    inputQ[i] = inputQ[i+1];
  inQLen--;
  return c;
}

void Keyboard::draw(Graphics *g)
{
  for(int i = 0; i < 38; i++)
  {
    keys[i].draw(g);
    glyphs[i].draw(g);
  }
  int k = 12;
  k += 5;
}

