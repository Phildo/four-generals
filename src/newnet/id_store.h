//uses large bitmap to manage a list of available IDs and
//oh god why am I doing this this is so unnecessary
#ifndef _FG_ID_STORE_H_
#define _FG_ID_STORE_H_

#define FG_ID_STORE_MAX_MASK (~((id_store_bitmask_t)0))
#define FG_ID_STORE_ONE_MASK ((id_store_bitmask_t)1)
#define FG_ID_STORE_MASK_LEN (sizeof(id_store_bitmask_t)*8)
#define FG_ID_STORE_ARRAY_LEN ((N/(sizeof(id_store_bitmask_t)*8))+1)

typedef unsigned int id_store_t;
typedef unsigned long id_store_bitmask_t;

template <id_store_t N>
class IdStore
{
  private:
    id_store_bitmask_t takenbits[FG_ID_STORE_ARRAY_LEN];
    bool idAvailable(id_store_t id);
  public:
    IdStore();
    id_store_t getId();
    void retireId(id_store_t id);
    bool test();
};

template <id_store_t N>
IdStore<N>::IdStore()
{
  for(int i = 0; i < FG_ID_STORE_ARRAY_LEN; i++) takenbits[i] = 0;
}

template <id_store_t N>
bool IdStore<N>::idAvailable(id_store_t id)
{
  int i = id/FG_ID_STORE_MASK_LEN;
  int j = id%FG_ID_STORE_MASK_LEN;
  if(id > N || takenbits[i] == FG_ID_STORE_MAX_MASK) return false;

  return (takenbits[i] & (FG_ID_STORE_ONE_MASK << j));
}

template <id_store_t N>
id_store_t IdStore<N>::getId()
{
  int i = 0;
  while(takenbits[i] == FG_ID_STORE_MAX_MASK && i < FG_ID_STORE_ARRAY_LEN)
    i++;
  if(i == FG_ID_STORE_ARRAY_LEN) return 0;

  int j = 0;
  while(takenbits[i] & (FG_ID_STORE_ONE_MASK << j))
    j++;

  takenbits[i] = takenbits[i] | (FG_ID_STORE_ONE_MASK << j);
  return (id_store_t)((i*FG_ID_STORE_MASK_LEN)+j);
}

template <id_store_t N>
void IdStore<N>::retireId(id_store_t id)
{
  int i = id/FG_ID_STORE_MASK_LEN;
  int j = id%FG_ID_STORE_MASK_LEN;
  if(id > N) return;

  takenbits[i] = takenbits[i] & ~(FG_ID_STORE_ONE_MASK << j);
}

template <id_store_t N>
bool IdStore<N>::test()
{
  for(int i = 0; i < 70; i++)
    if(getId() != i) return false;

  id_store_t t;
  t = 15; retireId(t); if(getId() != t) return false;
  t =  1; retireId(t); if(getId() != t) return false;
  t =  8; retireId(t); if(getId() != t) return false;
  t =  0; retireId(t); if(getId() != t) return false;
  t = 64; retireId(t); if(getId() != t) return false;
  t = 69; retireId(t); if(getId() != t) return false;

  return true;
}

#endif

