Keyboard::Keyboard() {}
Keyboard::Keyboard(int x, int y, int w, int h) : rect(x,y,w,h), keyw(rect.w/10), keyh(rect.h/4)
{
  initBoard();
}
Keyboard::Keyboard(SDL_Rect r) : rect(r), keyw(rect.w/10), keyh(rect.h/4)
{
  initBoard();
}

void Keyboard::initBoard()
{
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
  glyphs[i] = Label(&characters[i], 1, r);
  keys[i] = Button(r);
  i++;
  r.x += r.w;
}

void Keyboard::touch(In &in)
{
  for(int i = 0; i < 38; i++)
    if(keys[i].query(in)) enqueue(characters[i]);
}

void Keyboard::enqueue(char c)
{
  in_q.enqueue(c);
}

char Keyboard::poll()
{
  char *c = in_q.next();
  if(!c) return 0;
  return *c;
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

void Keyboard::drawInMask(Graphics *g, SDL_Rect m)
{
  for(int i = 0; i < 38; i++)
  {
    keys[i].drawInMask(g,m);
    glyphs[i].drawInMask(g,m);
  }
  int k = 12;
  k += 5;
}


