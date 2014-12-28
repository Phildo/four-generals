TextButton::TextButton() { }
TextButton::TextButton(const char *t, SDL_Rect r) : text(t), rect(r) { }
TextButton::TextButton(const char *t, int l, SDL_Rect r) : text(t, l), rect(r) {}
TextButton::TextButton(const char *t, int x, int y, int w, int h) : text(t), rect(x,y,w,h) {}
TextButton::TextButton(const char *t, int l, int x, int y, int w, int h) : text(t, l), rect(x,y,w,h) {}

bool TextButton::query(const In &in)
{
  return (in.x > rect.rect.x && in.x < rect.rect.x+rect.rect.w &&
          in.y > rect.rect.y && in.y < rect.rect.y+rect.rect.h);
}

void TextButton::draw(Graphics *g)
{
  SDL_Rect tmp = rect.rect;
  tmp.w = n_w*((float)rect.h/(float)n_h); //width of individual letter
  for(int i = 0; i < text.len(); i++)
  {
    g->draw(Sprite::alpha(*(text.ptr()+i)),tmp);
    tmp.x += tmp.w;
  }
  rect.draw(g);
}

