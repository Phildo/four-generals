/*
Array - 'Dynamically' sized array within statically sized space. written by Phil Dougherty
ultra-naive implementation. use at own idiocy.
*/

#ifndef _ARRAY_
#define _ARRAY_

template <typename T, unsigned int N>
class Array
{
  private:
    int len;
    T content[N];

    void expand();
  public:
    Array() : len(0) {}
    ~Array() {};

    T& operator[](int index)             { return content[index]; };
    const T& operator[](int index) const { return content[index]; };
    int length() const { return len; };
    void add(T t);
    void insert(int index, T t);
    int indexOf(T t);
    void remove(int index);
    void remove(T t);
    void removeAll();
};

template <typename T, unsigned int N>
void Array<T,N>::add(T t)
{
  if(len >= N) return; //don't overflow
  content[len] = t;
  len++;
}

template <typename T, unsigned int N>
void Array<T,N>::insert(int index, T t)
{
  if(len < N) content[len] = content[len-1]; //otherwise drop last member off end
  for(int i = len-1; i >= index; i--)
    content[i] = content[i-1];
  content[index] = t;
  len++;
}

template <typename T, unsigned int N>
int Array<T,N>::indexOf(T t)
{
  for(int i = 0; i < length(); i++)
    if(t == operator[](i)) return i;
  return -1;
}

template <typename T, unsigned int N>
void Array<T,N>::remove(int index) //definitely should implement with memcopy or something?
{
  for(int i = index; i < len; i++)
    content[i] = content[i+1];
  len--;
}

template <typename T, unsigned int N>
void Array<T,N>::remove(T t)
{
  int i = indexOf(t);
  if(i != -1) remove(i);
}

template <typename T, unsigned int N>
void Array<T,N>::removeAll()
{
  len = 0; //so easy!
}

#endif

