#ifndef GC_TESTS_FLIP_CORRECTNESS_TESTS_H
#define GC_TESTS_FLIP_CORRECTNESS_TESTS_H

#include "Test.h"
#include "gc.h"
#include "gc_test_access.h"

namespace FlipCorrectnessTests
{

struct A : public gc::object
{
    END_GC_FIELDS;
    int val;

    A(): val(0)
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

TEST(FlipBottomStandardObjectBecomesScan)
{

    gc::init(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a->val;

    ASSERT_BOTTOM(gc_test_access::debug_object(gc_test_access::debug_object(b)->_a));

    gc_test_access::collect_garbage();
    ASSERT_SCAN(gc_test_access::debug_object(gc_test_access::debug_object(b)->_a));


    TEST_SUCCESS;
}

TEST(FlipBottomStaticObjectBecomesTop)
{
    gc::init(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a;

    ASSERT_BOTTOM(gc_test_access::debug_object(b));

    gc_test_access::collect_garbage();
    ASSERT_TOP(gc_test_access::debug_object(b));

    TEST_SUCCESS;
}

TEST(FlipAccessStaticObjectBecomesBottom)
{
    gc::init(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a;

    ASSERT_BOTTOM(gc_test_access::debug_object(b));

    gc_test_access::collect_garbage();
    b->_a;

    ASSERT_BOTTOM(gc_test_access::debug_object(b));

    TEST_SUCCESS;
}

TEST(FlipAccessStandardObjectBecomesBottom)
{
    gc::init(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a->val;

    ASSERT_BOTTOM(gc_test_access::debug_object(gc_test_access::debug_object(b)->_a));

    gc_test_access::collect_garbage();
    b->_a->val;

    ASSERT_BOTTOM(gc_test_access::debug_object(gc_test_access::debug_object(b)->_a));

    TEST_SUCCESS;
}


int run_tests()
{
    using namespace FlipCorrectnessTests;

    RUN_TEST(FlipBottomStandardObjectBecomesScan);
    RUN_TEST(FlipBottomStaticObjectBecomesTop);
    RUN_TEST(FlipAccessStaticObjectBecomesBottom);
    RUN_TEST(FlipAccessStandardObjectBecomesBottom);

    TEST_SUCCESS;
}

}

#endif
