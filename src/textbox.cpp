Textbox::Textbox() {}
Textbox::Textbox(SDL_Rect r)
{
  textLen = 0;
  rect = r;
  label = Label(rect, text, 0);
}
Textbox::Textbox(int x, int y, int w, int h)
{
  textLen = 0;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  label = Label(rect, text, 0);
}

void Textbox::getString(char *&c)
{
  text[textLen] = '\0';
  strcpy(c, text);
}

void Textbox::input(char c)
{
  if(textLen >= FG_MAX_TEXTBOX_LEN) return; //ignore input
  text[textLen] = c;
  textLen++;
  label = Label(rect, text, textLen); //ouch- copy every time
}

void Textbox::backspace()
{
  if(textLen == 0) return;
  textLen--;
  label = Label(rect, text, textLen); //ouch- copy every time
}

void Textbox::clear()
{
  textLen = 0;
  label = Label(rect, "\0", 0); //ouch- copy every time
}

void Textbox::draw(Graphics *g)
{
  SDL_Rect tmp;

  tmp = Sprite::border_tl();
  tmp.x = rect.x;
  tmp.y = rect.y;
  g->draw(Sprite::border_tl(),tmp);

  tmp = Sprite::border_t();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.w = rect.w;
  g->draw(Sprite::border_t(), tmp);

  tmp = Sprite::border_tr();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  g->draw(Sprite::border_tr(),tmp);

  tmp = Sprite::border_r();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_r(), tmp);

  tmp = Sprite::border_br();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::border_br(),tmp);

  tmp = Sprite::border_b();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  tmp.w = rect.w;
  g->draw(Sprite::border_b(), tmp);

  tmp = Sprite::border_bl();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::border_bl(),tmp);

  tmp = Sprite::border_l();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_l(), tmp);

  label.draw(g);
}

