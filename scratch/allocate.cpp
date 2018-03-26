//clang++ -std=c++11 -I../include -mavx2 -mfma -mf16c -m64 -O3 -ffast-math allocate.cpp -o alloc

#include "simd.h"
#include <iostream>
#include <memory>


template <typename T, size_t align=16>
class AlignedArrayRAII
{
  public :
    AlignedArrayRAII(size_t _size) 
    {
      m_data = (T *)_mm_malloc(_size * sizeof(T),align);
      m_size=_size;
    }
    AlignedArrayRAII(){}
    AlignedArrayRAII(const AlignedArrayRAII &)=delete; 
    void reset(size_t _size)
    {
      if(m_data !=nullptr)
      {
        _mm_free(m_data);
      }
      m_data = (T *)_mm_malloc(_size * sizeof(T),align);
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

/*
template<typename T>
struct aligned_delete
{
  void operator()(T* ptr) const
  {
    _mm_free(ptr);
  }
};

template<class T> using unique_ptr_aligned = std::unique_ptr<T,aligned_delete<T>>;

template<class T>
unique_ptr_aligned<T> AlignedUniquePtr( size_t size, size_t align=16)
{
   return  unique_ptr_aligned<T>(static_cast<T*>(_mm_malloc( size,align)));
}
*/

int main()
{
//    std::unique_ptr<float [],aligned_delete<float>> a( (float *)_mm_malloc(1024,16));

  //   unique_ptr_aligned<float> b( AlignedUniquePtr<float>(1024,32));
  // unique_ptr_aligned<float> c;
  //   c=AlignedUniquePtr<float>(1024,32);
  AlignedArrayRAII<float,32> c;
  c.reset(1024);
    //( (float *)_mm_malloc(1024*sizeof(float),16) );  
     for(size_t i=0; i<1024; ++i)
        c[i]=i;
     for(size_t i=0; i<1024; ++i)
       std::cout<<c[i]<<'\n';

}