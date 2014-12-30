Label::Label() { }
Label::Label(const char *t, SDL_Rect r) : text(t), rect(r.x,r.y,n_w*((float)r.h/(float)n_h),r.h) { }
Label::Label(const char *t, int l, SDL_Rect r) : text(t, l), rect(r.x,r.y,n_w*((float)r.h/(float)n_h),r.h) {}
Label::Label(const char *t, int x, int y, int h) : text(t), rect(x,y,n_w*((float)h/(float)n_h),h) {}
Label::Label(const char *t, int l, int x, int y, int h) : text(t, l), rect(x,y,n_w*((float)h/(float)n_h),h) {}

void Label::draw(Graphics *g)
{
  SDL_Rect tmp = rect.rect;
  tmp.w = n_w*((float)rect.h/(float)n_h); //width of individual letter
  for(int i = 0; i < text.len(); i++)
  {
    g->draw(Sprite::alpha(*(text.ptr()+i)),tmp);
    tmp.x += tmp.w;
  }
}

