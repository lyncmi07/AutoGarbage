#ifndef GC_TESTS_REFERENCE_CHANGE_FLIP_CORRECTNESS_TESTS_H
#define GC_TESTS_REFERENCE_CHANGE_FLIP_CORRECTNESS_TESTS_H

#include "Test.h"

#include "gc.h"
#include "gc_test_access.h"

namespace ReferenceChangeFlipCorrectnessTests
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
        gc::field<A> a;
        END_GC_FIELDS;

        B(): a(new A())
        {
        }
    };

    TEST(ShouldCollectNoLongerReferencedObjectStaticPtr)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());
        B* b1 = gc_test_access::debug_object(ptr1);

        gc_test_access::collect_garbage();
        ASSERT_TOP(b1);

        gc::static_ptr<B> ptr2(new B());
        B* b2 = gc_test_access::debug_object(ptr2);
        ptr1 = ptr2;

        gc_test_access::collect_garbage();
        ASSERT_BOTTOM(b2);
        ASSERT_SCAN(b1);

        gc_test_access::collect_garbage();
        ASSERT_BOTTOM(b2); //In bottom after garbage collection because its pointer to by two pointers
        ASSERT_FREE(b1);

        TEST_SUCCESS;
    }

    TEST(ShouldCollectNoLongerReferencedObjectField)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());

        A* a1 = gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a);

        gc_test_access::collect_garbage();
        ASSERT_SCAN(a1);

        ptr1->a = new A();

        A* a2 = gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a);

        gc_test_access::collect_garbage();
        gc_test_access::collect_garbage(); // Collect again as a1 will have been black going to ecru on first collection
        ASSERT_FREE(a1);
        ASSERT_SCAN(a2);

        TEST_SUCCESS;
    }

    int run_tests()
    {
        using namespace ReferenceChangeFlipCorrectnessTests;

        RUN_TEST(ShouldCollectNoLongerReferencedObjectStaticPtr);
        RUN_TEST(ShouldCollectNoLongerReferencedObjectField);

        TEST_SUCCESS;
    }
}
#endif
