Scroller::Scroller() { }
Scroller::Scroller(SDL_Rect r, SDL_Rect c) : rect(r), content_rect(c), button(rect.rect)
{
  down = false;
  down_time = 0;
  last_y = 0;
  offset = 0;
}
Scroller::Scroller(int rx, int ry, int rw, int rh, int cx, int cy, int cw, int ch) : rect(rx,ry,rw,rh), content_rect(cx,cy,cw,ch), button(rect.rect)
{
  down = false;
  down_time = 0;
  last_y = 0;
  offset = 0;
}

void Scroller::touch(const In &in)
{
  if(in.type == In::DOWN && button.query(in))
  {
    down = true;
    down_time = 0;
    last_y = in.y;
  }
  else if(in.type == In::MOVE && down && button.query(in))
  {
    offset += in.y-last_y;
    if(offset > 0) offset = 0;
    if(offset < (-content_rect.h+rect.h)) offset = -content_rect.h+rect.h;
    last_y = in.y;
  }
  else if(in.type == In::UP)
  {
    down = false;
  }
}

void Scroller::tick()
{
  if(down) down_time++;
}

void Scroller::draw(Graphics *g)
{
  rect.draw(g);
}

float Scroller::p()
{
  return ((float)offset)/((float)(-content_rect.h+rect.h));
}

