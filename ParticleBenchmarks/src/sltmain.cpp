#include <sltbench/Bench.h>
#include "ParticleSystemSSE.h"
#include "ParticleSystemSSEFMA.h"
#include "ParticleSystemNormal.h"
#include "ParticleSystemAOS.h"


template<typename T>
class ParticleSystemFixture
{
 public:
    ParticleSystemFixture(){}
  void SetUp()
  {

    particles= new T(st.range(0),{0,0,0});
  }

  void TearDown()
  {
    delete particles;
  }
  T *particles;

};



int main(int argc, char **argv)
{
  // Set up the main runner.
  ::hayai::MainRunner runner;
  // Parse the arguments.
  int result = runner.ParseArgs(argc, argv);
  if (result)
      return result;

  // Execute based on the selected mode.
  return runner.Run();
}
