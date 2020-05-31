#ifndef GC_TESTS_ALLOCATION_CORRECTNESS_TESTS_H
#define GC_TESTS_ALLOCATION_CORRECTNESS_TESTS_H

#include "Test.h"

#include "gc.h"

namespace AllocationCorrectnessTests
{

struct A : public gc::object
{
    END_GC_FIELDS;
    int a;

    A():
        a(1)
    {
    }
};

struct B : public gc::object
{
    gc::field<A> _a;
    END_GC_FIELDS;

    B(): _a(new A())
    {

    }
};

TEST(StaticPtrAllocationObjectIsInTopList)
{
    gc::init(4096, 25);

    gc::static_ptr<A> a(new A());
    ASSERT_TOP(gc_test_access::debug_object(a));

    TEST_SUCCESS;
}

TEST(StandardAllocationObjectIsInScanList)
{
    gc::init(4096, 25);

    gc::static_ptr<B> b(new B());
    ASSERT_SCAN(gc_test_access::debug_object(gc_test_access::debug_object(b)->_a));
    ASSERT_SCAN(gc_test_access::debug_object(gc_test_access::debug_object(b)->_a));

    TEST_SUCCESS;
}

int run_tests()
{
    using namespace AllocationCorrectnessTests;

    RUN_TEST(StaticPtrAllocationObjectIsInTopList);
    RUN_TEST(StandardAllocationObjectIsInScanList);

    TEST_SUCCESS;
}

}

#endif
