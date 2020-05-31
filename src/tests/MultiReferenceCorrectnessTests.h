#ifndef GC_TESTS_MULTI_REFERENCE_CORRECTENESS_TESTS_H
#define GC_TESTS_MULTI_REFERENCE_CORRECTENESS_TESTS_H

#include "Test.h"
#include "gc.h"
#include "gc_test_access.h"

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
        gc::field<A> a;
        END_GC_FIELDS;

        B(): a(new A())
        {
        }

        B(gc::field<A>& a_original)
            : a(a_original)
        {
        }
    };

    TEST(ShouldNotCollectDoubleReferencedObject)
    {
        gc::init(4096, 25);

        B* b = new B();
        {
            {
                gc::static_ptr<B> ptr1(b);

                gc_test_access::collect_garbage();
                ASSERT_TOP(b);
            }

            gc::static_ptr<B> ptr2(b);

            gc_test_access::collect_garbage();
            ASSERT_TOP(b);
        }

        TEST_SUCCESS;
    }

    TEST(ShouldNotCollectWithTwoReferencesStaticPtr)
    {
        gc::init(4096, 25);

        B* b = new B();
        gc::static_ptr<B> ptr1(b);
        ASSERT_TOP(b);

        gc_test_access::collect_garbage();

        gc::static_ptr<B> ptr2(b);

        gc_test_access::collect_garbage();
        ASSERT_BOTTOM(b);

        TEST_SUCCESS;
    }

    TEST(ShouldNotAffectObjectOnCopyConstructionStaticPtr)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());
        ASSERT_TOP(gc_test_access::debug_object(ptr1));

        gc::static_ptr<B> ptr2(ptr1);
        ASSERT_BOTTOM(gc_test_access::debug_object(ptr1));
        ASSERT_BOTTOM(gc_test_access::debug_object(ptr2));
        ASSERT_EQUALS(gc_test_access::debug_object(ptr1), gc_test_access::debug_object(ptr2));

        TEST_SUCCESS;
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualStaticPtr)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());
        gc::static_ptr<B> ptr2;

        ptr2 = ptr1;
        ASSERT_TOP(gc_test_access::debug_object(ptr1));
        ASSERT_TOP(gc_test_access::debug_object(ptr2)); // Adding to another ptr will cause gc_mark to be called moving the object to bottom
        ASSERT_EQUALS(gc_test_access::debug_object(ptr1), gc_test_access::debug_object(ptr2));

        TEST_SUCCESS;
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualObjectToStaticPtr)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1 = new B();
        gc::static_ptr<B> ptr2;

        ptr2 = gc_test_access::debug_object(ptr1);
        ASSERT_TOP(gc_test_access::debug_object(ptr1));

        TEST_SUCCESS;
    }

    TEST(ShouldNotCollectWithTwoReferencesField)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1 = new B();
        gc::static_ptr<B> ptr2 = new B();

        ptr2->a = ptr1->a;
        ASSERT_TOP(gc_test_access::debug_object(gc_test_access::debug_object(ptr2)->a));
        ASSERT_EQUALS(gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a), gc_test_access::debug_object(gc_test_access::debug_object(ptr2)->a));

        TEST_SUCCESS;
    }

    TEST(ShouldNotAffectObjectOnCopyConstructionField)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());
        ASSERT_SCAN(gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a));

        gc::static_ptr<B> ptr2(new B(ptr1->a));
        ASSERT_TOP(gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a));
        ASSERT_EQUALS(gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a), gc_test_access::debug_object(gc_test_access::debug_object(ptr2)->a));

        TEST_SUCCESS;
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualField)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());
        gc::static_ptr<B> ptr2(new B());

        ptr2->a = ptr1->a;
        ASSERT_BOTTOM(gc_test_access::debug_object(ptr2));
        ASSERT_EQUALS(gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a), gc_test_access::debug_object(gc_test_access::debug_object(ptr2)->a));

        TEST_SUCCESS;
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualObjectToField)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> ptr1(new B());
        gc::static_ptr<B> ptr2(new B());

        ptr2->a = gc_test_access::debug_object(ptr1)->a;
        ASSERT_BOTTOM(gc_test_access::debug_object(ptr2));
        ASSERT_EQUALS(gc_test_access::debug_object(gc_test_access::debug_object(ptr1)->a), gc_test_access::debug_object(gc_test_access::debug_object(ptr2)->a));

        TEST_SUCCESS;
    }

int run_tests()
{
    using namespace MultiReferenceCorrectnessTests;

    RUN_TEST(ShouldNotCollectDoubleReferencedObject);
    RUN_TEST(ShouldNotCollectWithTwoReferencesStaticPtr);
    RUN_TEST(ShouldNotAffectObjectOnCopyConstructionStaticPtr);
    RUN_TEST(ShouldNotAffectObjectOnOperatorEqualStaticPtr)
    RUN_TEST(ShouldNotAffectObjectOnOperatorEqualObjectToStaticPtr)
    RUN_TEST(ShouldNotCollectWithTwoReferencesField)
    RUN_TEST(ShouldNotAffectObjectOnCopyConstructionField)
    RUN_TEST(ShouldNotAffectObjectOnOperatorEqualField)
    RUN_TEST(ShouldNotAffectObjectOnOperatorEqualObjectToField)

    TEST_SUCCESS;
}
}

#endif
