#include "Test.h"
#include "FlipCorrectnessTests.h"
#include "AllocationCorrectnessTests.h"

int main()
{
    std::cout << std::endl << "---RUNNING TESTS---" << std::endl;
    RUN_TEST_SUITE(AllocationCorrectnessTests);
    RUN_TEST_SUITE(FlipCorrectnessTests);
}


