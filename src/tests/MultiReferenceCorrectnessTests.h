#ifndef GC_TESTS_MULTI_REFERENCE_CORRECTENESS_TESTS_H
#define GC_TESTS_MULTI_REFERENCE_CORRECTENESS_TESTS_H

#include "Test.h"
#include "gc.h"

namespace MultiReferenceCorrectnessTests
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

    TEST(ShouldNotCollectDoubleReferencedObject)
    {
        gc::heap::heap_struct::init_gc(4096);

        B* b = new B();
        {
            {
                gc::static_ptr<B> ptr1(b);

                gc::heap::heap_struct::get()->collect_garbage();
                ASSERT_TOP(b);
            }

            gc::static_ptr<B> ptr2(b);

            gc::heap::heap_struct::get()->collect_garbage();
            ASSERT_TOP(b);
        }
    }

    TEST(ShouldNotChangeTreadmillPositionOnSecondReference)
    {
        gc::heap::heap_struct::init_gc(4096);

        B* b = new B();
        gc::static_ptr<B> ptr1(b);
        ASSERT_TOP(b);

        gc::static_ptr<B> ptr2(b);
        ASSERT_BOTTOM(b); //gc_mark is called on a static ptr so this is moved to the bottom list
    }

int run_tests()
{
    using namespace MultiReferenceCorrectnessTests;

    RUN_TEST(ShouldNotCollectDoubleReferencedObject);
    RUN_TEST(ShouldNotChangeTreadmillPositionOnSecondReference);
}
}

#endif
