#include <iostream>
#include <chrono>
#include <thread>

void render()
{
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(10ms);  
}

int main()
{
  std::chrono::milliseconds renderTime;
  auto begin = std::chrono::steady_clock::now();
  // some work that takes time
  render();
  auto end = std::chrono::steady_clock::now();
  renderTime=std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);
  std::cout<<"Render Time is "<<renderTime.count()<<"ms\n";
}
