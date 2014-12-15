#include "string.h"

String::String() : local(""), handle(&local[0]), length(0) { }

String::String(const String &other) //copy
{
  char *n;
  length = other.len();
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = other.ptr()[i];
  n[length] = '\0';
  handle = n;
}
String &String::operator=(const String &other)
{
  char *n;
  length = other.len();
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = other.ptr()[i];
  n[length] = '\0';
  handle = n;
  return *this;
}

String::String(String &&other) //move
{
  length = other.len();
  if(length >= FG_STRING_LOCAL_LENGTH)
  {
    handle = other.ptr();
    other.handle = &other.local[0];
  }
  else
  {
    for(int i = 0; i < length; i++)
      local[i] = other.ptr()[i];
    local[length] = '\0';
    handle = &local[0];
  }
}
String::String(const char* s, int l)
{
  char *n;
  length = l;
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = s[i];
  n[length] = '\0';
  handle = n;
}
String::String(const char* s) //derive length from string
{
  char *n;
  for(length = 0; length < 1000 && s[length] != '\0'; length++) ; //find length, max out @ 1000
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = s[i];
  n[length] = '\0';
  handle = n;
}

String::~String()
{
  if(handle != &local[0])
    delete[] handle;
}

int String::len() const
{
  return length;
}

const char *String::ptr() const
{
  return handle;
}


String String::concat(const String &other) const
{
  int newl = len()+other.len();
  char *buff = new char[newl]; //expensive!

  for(int i = 0; i < newl; i++)
  {
    if(i < len()) *(buff+i) = *(ptr()+i);
    else          *(buff+i) = *(other.ptr()+(i-len()));
  }
  return String(buff, newl);
}


String String::decimalRep(int i)
{
  int j = i;
  int d = 0;
  while(j > 0)
  {
    j/=10;
    d++;
  }
  return decimalRep(i, d);
}

int String::intVal()
{
  int v = 0;
  for(int i = 0; i < len(); i++)
  {
    v *= 10;
    v += ((int)(*(ptr()+i)-'0'));
  }
  return v;
}

String String::decimalRep(int i, int digs)
{
  if(digs <= 0) digs = 1;

  char rep[32];
  char *r;
  if(digs > 32) r = new char[digs]; //only put it on heap if necessary
  else r = &rep[0];

  int d = 0;
  for(int j = digs-1; j >= 0; j--)
  {
    d = i%10;
    r[j] = '0'+d;
    i /= 10;
  }

  String s(r, digs);
  if(r != &rep[0]) delete[] r;
  return s;
}

