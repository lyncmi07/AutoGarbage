#ifndef GC_TESTS_FLIP_CORRECTNESS_TESTS_H
#define GC_TESTS_FLIP_CORRECTNESS_TESTS_H

#include "Test.h"
#include "gc.h"

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

    gc::heap::heap_struct::init_gc(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a->val;

    ASSERT_BOTTOM(b.debug_object()->_a.debug_object());

    gc::heap::heap_struct::get()->collect_garbage();
    ASSERT_SCAN(b.debug_object()->_a.debug_object());


    TEST_SUCCESS;
}

TEST(FlipBottomStaticObjectBecomesTop)
{
    gc::heap::heap_struct::init_gc(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a;

    ASSERT_BOTTOM(b.debug_object());

    gc::heap::heap_struct::get()->collect_garbage();
    ASSERT_TOP(b.debug_object());

    TEST_SUCCESS;
}

TEST(FlipAccessStaticObjectBecomesBottom)
{
    gc::heap::heap_struct::init_gc(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a;

    ASSERT_BOTTOM(b.debug_object());

    gc::heap::heap_struct::get()->collect_garbage();
    b->_a;

    ASSERT_BOTTOM(b.debug_object());

    TEST_SUCCESS;
}

TEST(FlipAccessStandardObjectBecomesBottom)
{
    gc::heap::heap_struct::init_gc(4096, 25);

    gc::static_ptr<B> b(new B());
    b->_a->val;

    ASSERT_BOTTOM(b.debug_object()->_a.debug_object());

    gc::heap::heap_struct::get()->collect_garbage();
    b->_a->val;

    ASSERT_BOTTOM(b.debug_object()->_a.debug_object());

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
