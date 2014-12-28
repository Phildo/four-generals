Button::Button() { }
Button::Button(SDL_Rect r) : rect(r) { }
Button::Button(int x, int y, int w, int h) : rect(x,y,w,h) { }

bool Button::query(const In &in)
{
  return (in.x > rect.rect.x && in.x < rect.rect.x+rect.rect.w &&
          in.y > rect.rect.y && in.y < rect.rect.y+rect.rect.h);
}

void Button::draw(Graphics *g)
{
  rect.draw(g);
}

