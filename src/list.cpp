List::List() { }
List::List(SDL_Rect r, int t_h)
{
  rect = r;
  text_height = t_h;
  num_rows = r.h/t_h;
  if(num_rows > max_rows) num_rows = max_rows;

  for(int i = 0; i < num_rows; i++)
    rows[i] = Label(rect.x, rect.y+(i*text_height), text_height, "");
}
List::List(int x, int y, int w, int h, int t_h)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  text_height = t_h;
  num_rows = rect.h/t_h;
  if(num_rows > max_rows) num_rows = max_rows;

  for(int i = 0; i < num_rows; i++)
    rows[i] = Label(rect.x, rect.y+(i*text_height), text_height, "");
}

void List::draw(Graphics *g)
{
  for(int i = 0; i < num_rows; i++)
    rows[i].draw(g);
}

void List::append(const char *b)
{
  for(int i = 0; i < num_rows-1; i++)
    rows[i].text = rows[i+1].text;
  rows[num_rows-1].text = String(b);
}

