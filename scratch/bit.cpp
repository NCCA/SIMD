#include <bitset>
#include <iostream>
template <size_t N>
std::bitset<N> increment ( std::bitset<N> in ) 
{
//  add 1 to each value, and if it was 1 already, carry the 1 to the next.
    for ( size_t i = 0; i < N; ++i ) {
        if ( in[i] == 0 ) {  // There will be no carry
            in[i] = 1;
            break;
            }
        in[i] = 0;  // This entry was 1; set to zero and carry the 1
        }
    return in;
}

int main()
{
  std::bitset<8> d;
  for(int i=0; i<256; ++ i)
  {
    std::cout<<d<<'\n';
    d=increment(d);
  }
}