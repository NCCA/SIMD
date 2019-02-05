#ifndef ALIGNEDARRAY_H_
#define ALIGNEDARRAY_H_

#include "simd.h"
#include <cstring>
#include <cassert>
#include <memory>
#include <algorithm>

// todo update this to use the new C++ 17 new(std::align_val_t(align))
// and also check how we can do this with make_unique
template <typename T, size_t align>
class AlignedArrayRAII
{
  public :
    AlignedArrayRAII(size_t _size)
    {
      m_data = static_cast<T *>(_mm_malloc(_size * sizeof(T),align));
      m_size=_size;
    }
    AlignedArrayRAII(){}
    AlignedArrayRAII(const AlignedArrayRAII & _c)
    {
      m_data = static_cast<T *>(_mm_malloc(_c.m_size * sizeof(T),align));
      m_size=_c.m_size;
      std::memcpy(m_data,_c.m_data,_c.m_size * sizeof(T));
    }
    void reset(size_t _size)
    {
      if(m_data !=nullptr)
      {
        _mm_free(m_data);
      }
      m_data = static_cast<T *>(_mm_malloc(_size * sizeof(T),align));
      m_size=_size;

    }
    ~AlignedArrayRAII(){ _mm_free(m_data);}
    T &operator[](size_t _index)
    {
      return m_data[_index];
    }

  private :
    T *m_data=nullptr;
    size_t m_size=0;
};

#endif
