TextBox::TextBox() {}
TextBox::TextBox(SDL_Rect r)
{
  txt_len = 0;
  rect = r;
  label = Label(rect, txt, 0);
}
TextBox::TextBox(int x, int y, int w, int h)
{
  txt_len = 0;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  label = Label(rect, txt, 0);
}

void TextBox::setText(String t)
{
  txt_len = t.len();
  strcpy(&txt[0], t.ptr());
  label.text = String(txt, txt_len);
}

String TextBox::getText()
{
  return String(txt, txt_len);
}

void TextBox::input(char c)
{
  if(txt_len >= FG_MAX_TEXTBOX_LEN-1) return; //ignore input (save one for '\0')
  txt[txt_len] = c;
  txt_len++;
  label.text = String(txt, txt_len);
}

void TextBox::backspace()
{
  if(txt_len == 0) return;
  txt_len--;
  label.text = String(txt, txt_len);
}

void TextBox::clear()
{
  txt_len = 0;
  label.text = String(txt, txt_len);
}

void TextBox::draw(Graphics *g)
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

