#include "string.h"

String::String() : local("\0"), handle(0), length(0) { }

String::String(const String &other) //copy
{
  local[0] = '\0';
  length = other.len();
  char *n;
  if(length > FG_STRING_LOCAL_LENGTH) n = new char[length];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = other.ptr()[i];
  handle = n;
}
String &String::operator=(const String &other)
{
  local[0] = '\0';
  length = other.len();
  char *n;
  if(length > FG_STRING_LOCAL_LENGTH) n = new char[length];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = other.ptr()[i];
  handle = n;
  return *this;
}

String::String(String &&other) //move
{
  local[0] = '\0';
  length = other.len();
  if(length > FG_STRING_LOCAL_LENGTH)
  {
    handle = other.ptr();
    other.handle = &other.local[0];
  }
  else
  {
    for(int i = 0; i < length; i++)
      local[i] = other.ptr()[i];
    handle = &local[0];
  }
}
String::String(const char* s, int l)
{
  local[0] = '\0';
  length = l;
  char *n;
  if(length > FG_STRING_LOCAL_LENGTH) n = new char[length];
  else                                 n = &local[0];

  for(int i = 0; i < l; i++)
    n[i] = s[i];
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

