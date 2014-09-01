/*
sArray - variable length array written by Phil Dougherty
Note- unsafe and novice as hell. use only if you are an idiot.
Also, currently straight up broken. operator[] doesn't effect shadow array.
tl;dr: don't use.
*/

#ifndef _SHADOW_ARRAY_
#define _SHADOW_ARRAY_

#define DEFAULT_SIZE 5

template <typename T>
class sArray
{
  private:
    T* content;
    T* shadow; //always double size of content
    int c_size; //size allocated for content
    int len; //length used

    void RA();
  public:
    sArray()               : len(0), c_size(DEFAULT_SIZE), content(new T[c_size]), shadow(new T[c_size*2]) {}
    sArray(int start_size) : len(0), c_size(start_size),   content(new T[c_size]), shadow(new T[c_size*2]) {}
    ~sArray() { delete[] content; delete[] shadow; }

    T& operator[](int index)             { return content[index]; }
    const T& operator[](int index) const { return content[index]; }
    int length() { return len; }
    void add(T content);
    void remove(int index);
};

template <typename T>
void sArray<T>::add(T t)
{
  content[len] = t;
  if(len >= c_size/2) //unfortunate that this check is only relevant for first start_size/2 adds...
  {
    int s_i = c_size-(2*(c_size-len));
    shadow[s_i] = content[s_i];
    shadow[s_i+1] = content[s_i+1];
  }

  len++;

  if(len == c_size)
  {
    delete[] content;
    content = shadow;
    c_size *= 2;
    shadow = new T[c_size*2];
  }
}

template <typename T>
void sArray<T>::remove(int index) //maybe should implement with memcopy or something?
{
  for(int i = index+1; i < len; i++)
    content[i] = content[i+1];
  for(int i = index+1; i < len; i++) //could check to make sure i is within already copied shadow, and not do this if fail, but ef it
    shadow[i] = shadow[i+1];

  len--;
}

#endif

