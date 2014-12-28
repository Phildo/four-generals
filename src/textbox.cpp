TextBox::TextBox() {}
TextBox::TextBox(SDL_Rect r) : txt_len(0), rect(r) { }
TextBox::TextBox(int x, int y, int w, int h) : txt_len(0), rect(x,y,w,h) { }

void TextBox::setText(String t)
{
  txt_len = t.len();
  strcpy(&txt[0], t.ptr());
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
}

void TextBox::backspace()
{
  if(txt_len == 0) return;
  txt_len--;
}

void TextBox::clear()
{
  txt_len = 0;
}

void TextBox::draw(Graphics *g)
{
  SDL_Rect tmp = rect.rect;
  tmp.w = n_w*((float)rect.h/(float)n_h); //width of individual letter
  for(int i = 0; i < txt_len; i++)
  {
    g->draw(Sprite::alpha(txt[i]),tmp);
    tmp.x += tmp.w;
  }
  rect.draw(g);
}

