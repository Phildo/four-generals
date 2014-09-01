/*
vArray - variable length array written by Phil Dougherty
ultra-naive implementation. use at own idiocy.
*/

#ifndef _VAR_ARRAY_
#define _VAR_ARRAY_

#define VAR_ARRAY_DEFAULT_SIZE 5

template <typename T>
class vArray
{
  private:
    int len; //length used
    int a_size; //size allocated for content
    T* content;

    void expand();
    void RA();
  public:
    vArray()               : len(0), a_size(VAR_ARRAY_DEFAULT_SIZE), content(new T[a_size]) {}
    vArray(int start_size) : len(0), a_size(start_size),             content(new T[a_size]) {}
    ~vArray() { delete[] content; }

    T& operator[](int index)             { return content[index]; };
    const T& operator[](int index) const { return content[index]; };
    int length() const { return len; };
    void add(T t);
    void insert(int index, T t);
    void remove(int index);
    void removeAll();
};

template <typename T>
void vArray<T>::expand()
{
  T* tmp = new T[a_size*2];
  for(int i = 0; i < a_size; i++)
    tmp[i] = content[i];
  delete[] content;
  content = tmp;
  a_size *= 2;
}

template <typename T>
void vArray<T>::add(T t)
{
  if(len >= a_size) expand();
  content[len] = t;
  len++;
}

template <typename T>
void vArray<T>::insert(int index, T t)
{
  if(len >= a_size) expand();
  for(int i = len-1; i >= index; i--)
    content[i+1] = content[i];
  content[index] = t;
  len++;
}

template <typename T>
void vArray<T>::remove(int index) //maybe should implement with memcopy or something?
{
  for(int i = index; i < len; i++)
    content[i] = content[i+1];
  len--;
}

template <typename T>
void vArray<T>::removeAll()
{
  len = 0; //so easy!
}

#endif

