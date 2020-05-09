#include "Test.h"
#include "FlipCorrectnessTests.h"
#include "AllocationCorrectnessTests.h"
#include "MultiReferenceCorrectnessTests.h"
#include "ReferenceChangeFlipCorrectnessTests.h"

int main()
{
    std::cout << std::endl << "---RUNNING TESTS---" << std::endl;
    RUN_TEST_SUITE(AllocationCorrectnessTests);
    RUN_TEST_SUITE(FlipCorrectnessTests);
    RUN_TEST_SUITE(MultiReferenceCorrectnessTests);
    RUN_TEST_SUITE(ReferenceChangeFlipCorrectnessTests);

    TEST_SUCCESS;
}

void debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
}
