#ifndef ALIGNEDUNIQUEPTR_H_
#define ALIGNEDUNIQUEPTR_H_

#include <memory>
#include <immintrin.h>

template<typename T>
struct aligned_delete
{
  void operator()(T* ptr) const
  {
    _mm_free(ptr);
  }
};


template<class T> using unique_ptr_aligned = std::unique_ptr<T,decltype(&_mm_free)>;

template<class T>
unique_ptr_aligned<T> AlignedUniquePtr( size_t size, size_t align=16)
{
   return  unique_ptr_aligned<T>(
         static_cast<T*>(_mm_malloc( size,align)),
         &_mm_free);
}

template<class T []>
unique_ptr_aligned<T []> AlignedUniquePtr( size_t size, size_t align=16)
{
   return  unique_ptr_aligned<T []>(
         static_cast<T*>(_mm_malloc( size,align)),
         &_mm_free);
}





#endif
