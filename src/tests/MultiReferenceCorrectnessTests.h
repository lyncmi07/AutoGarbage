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

    TEST(ShouldNotCollectWithTwoReferences)
    {
        gc::heap::heap_struct::init_gc(4096);

        B* b = new B();
        gc::static_ptr<B> ptr1(b);
        ASSERT_TOP(b);

        gc::heap::heap_struct::get()->collect_garbage();

        gc::static_ptr<B> ptr2(b);

        gc::heap::heap_struct::get()->collect_garbage();
        ASSERT_BOTTOM(b);
    }

    TEST(ShouldNotAffectObjectOnCopyConstructionStaticPtr)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1(new B());
        ASSERT_TOP(ptr1.debug_object());

        gc::static_ptr<B> ptr2(ptr1);
        ASSERT_BOTTOM(ptr1.debug_object());
        ASSERT_BOTTOM(ptr2.debug_object());
        ASSERT_EQUALS(ptr1.debug_object(), ptr2.debug_object());
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualStaticPtr)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1(new B());
        gc::static_ptr<B> ptr2;

        ptr2 = ptr1;
        ASSERT_TOP(ptr1.debug_object());
        ASSERT_TOP(ptr2.debug_object()); // Adding to another ptr will cause gc_mark to be called moving the object to bottom
        ASSERT_EQUALS(ptr1.debug_object(), ptr2.debug_object());
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualObjectToStaticPtr)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1 = new B();
        ASSERT_TOP(ptr1.debug_object());
    }

int run_tests()
{
    using namespace MultiReferenceCorrectnessTests;

    RUN_TEST(ShouldNotCollectDoubleReferencedObject);
    RUN_TEST(ShouldNotCollectWithTwoReferences);
    RUN_TEST(ShouldNotAffectObjectOnCopyConstructionStaticPtr);
    RUN_TEST(ShouldNotAffectObjectOnOperatorEqualStaticPtr)
    RUN_TEST(ShouldNotAffectObjectOnOperatorEqualObjectToStaticPtr)
}
}

#endif
