#ifndef GC_TESTS_ALLOCATION_CORRECTNESS_TESTS_H
#define GC_TESTS_ALLOCATION_CORRECTNESS_TESTS_H

#include "Test.h"

#include "gc.h"

struct A : public gc::object
{
    END_GC_FIELDS;
};

struct B : public gc::object
{
    gc::field<A> _a;

    B(): _a(new A())
    {

    }
};

TEST(StaticPtrAllocationObjectIsInTopList)
{
    gc::heap::heap_struct::init_gc(4096);

    gc::static_ptr<A> a(new A());
    ASSERT_TOP(a.debug_object());

    TEST_SUCCESS;
}

TEST(StandardAllocationObjectIsInScanList)
{
    gc::heap::heap_struct::init_gc(4096);

    gc::static_ptr<B> b(new B());
    ASSERT_SCAN(b.debug_object()->_a.debug_object());

    TEST_SUCCESS;
}

TEST(AllocationCorrectnessTests)
{
    RUN_TEST(StaticPtrAllocationObjectIsInTopList);
    RUN_TEST(StandardAllocationObjectIsInScanList);
}

#endif
