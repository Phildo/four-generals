Button::Button() { }
Button::Button(SDL_Rect r)
{
  box = Box(r);
}
Button::Button(int x, int y, int w, int h)
{
  box = Box(x,y,w,h);
}

bool Button::query(const In &in)
{
  return (in.x > box.rect.x && in.x < box.rect.x+box.rect.w &&
          in.y > box.rect.y && in.y < box.rect.y+box.rect.h);
}

void Button::draw(Graphics *g)
{
  box.draw(g);
}

