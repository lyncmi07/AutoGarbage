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

    TEST(ShouldNotCollectWithTwoReferencesStaticPtr)
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
        gc::static_ptr<B> ptr2;

        ptr2 = ptr1.debug_object();
        ASSERT_TOP(ptr1.debug_object());
    }

    TEST(ShouldNotCollectWithTwoReferencesField)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1 = new B();
        gc::static_ptr<B> ptr2 = new B();

        ptr2->a = ptr1->a;
        ASSERT_TOP(ptr2.debug_object()->a.debug_object());
        ASSERT_EQUALS(ptr1.debug_object()->a.debug_object(), ptr2.debug_object()->a.debug_object());
    }

    TEST(ShouldNotAffectObjectOnCopyConstructionField)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1(new B());
        ASSERT_SCAN(ptr1.debug_object()->a.debug_object());

        gc::static_ptr<B> ptr2(new B(ptr1->a));
        ASSERT_TOP(ptr1.debug_object()->a.debug_object());
        ASSERT_EQUALS(ptr1.debug_object()->a.debug_object(), ptr2.debug_object()->a.debug_object());
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualField)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1(new B());
        gc::static_ptr<B> ptr2(new B());

        ptr2->a = ptr1->a;
        ASSERT_BOTTOM(ptr2.debug_object());
        ASSERT_EQUALS(ptr1.debug_object()->a.debug_object(), ptr2.debug_object()->a.debug_object());
    }

    TEST(ShouldNotAffectObjectOnOperatorEqualObjectToField)
    {
        gc::heap::heap_struct::init_gc(4096);

        gc::static_ptr<B> ptr1(new B());
        gc::static_ptr<B> ptr2(new B());

        ptr2->a = ptr1->a.debug_object();
        ASSERT_BOTTOM(ptr2.debug_object());
        ASSERT_EQUALS(ptr1.debug_object()->a.debug_object(), ptr2.debug_object()->a.debug_object());
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
}
}

#endif
