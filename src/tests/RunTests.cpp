#include "gc_scoped_timer.h"
#include "Test.h"
#include "FlipCorrectnessTests.h"
#include "AllocationCorrectnessTests.h"
#include "MultiReferenceCorrectnessTests.h"
#include "ReferenceChangeFlipCorrectnessTests.h"
#include "ArrayTests.h"

int main()
{
    #if (PERFORMANCE_TIMERS)
        scoped_timer::init_timers();
    #endif

    std::cout << std::endl << "---RUNNING TESTS---" << std::endl;
    RUN_TEST_SUITE(AllocationCorrectnessTests);
    RUN_TEST_SUITE(FlipCorrectnessTests);
    RUN_TEST_SUITE(MultiReferenceCorrectnessTests);
    RUN_TEST_SUITE(ReferenceChangeFlipCorrectnessTests);
    RUN_TEST_SUITE(ArrayTests);

    #if (PERFORMANCE_TIMERS)
        scoped_timer::print_info();
    #endif

    TEST_SUCCESS;
}

void debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
}
