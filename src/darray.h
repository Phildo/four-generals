/*
DArray - 'Dynamically' sized array within statically sized space. written by Phil Dougherty
ultra-naive implementation. use at own idiocy.
*/

#ifndef _DARRAY_
#define _DARRAY_

template <typename T, unsigned int N>
class DArray
{
  private:
    int length;
    T content[N];

    void expand();
  public:
    DArray() : length(0) {}
    ~DArray() {};

    T& operator[](int index)             { return content[index]; };
    const T& operator[](int index) const { return content[index]; };
    int len() const { return length; };
    void add(T t);
    void insert(int index, T t);
    int indexOf(T t);
    void remove(int index);
    void remove(T t);
    void removeAll();
};

template <typename T, unsigned int N>
void DArray<T,N>::add(T t)
{
  if(length >= N) return; //don't overflow
  content[length] = t;
  length++;
}

template <typename T, unsigned int N>
void DArray<T,N>::insert(int index, T t)
{
  if(length < N) content[length] = content[length-1]; //otherwise drop last member off end
  for(int i = length-1; i >= index; i--)
    content[i] = content[i-1];
  content[index] = t;
  length++;
}

template <typename T, unsigned int N>
int DArray<T,N>::indexOf(T t)
{
  for(int i = 0; i < length; i++)
    if(t == operator[](i)) return i;
  return -1;
}

template <typename T, unsigned int N>
void DArray<T,N>::remove(int index) //definitely should implement with memcopy or something?
{
  for(int i = index; i < length; i++)
    content[i] = content[i+1];
  length--;
}

template <typename T, unsigned int N>
void DArray<T,N>::remove(T t)
{
  int i = indexOf(t);
  if(i != -1) remove(i);
}

template <typename T, unsigned int N>
void DArray<T,N>::removeAll()
{
  length = 0; //so easy!
}

#endif

