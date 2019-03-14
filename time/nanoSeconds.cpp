#include <iostream>
#include <chrono>
#include <thread>

void render()
{
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1ms);  
}

int main()
{
  std::chrono::nanoseconds renderTime;
  auto begin = std::chrono::high_resolution_clock::now();
  // some work that takes time
  render();
  auto end = std::chrono::high_resolution_clock::now();
  renderTime=std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin);
  std::cout<<"Render Time is "<<renderTime.count()<<"ns\n";
}
