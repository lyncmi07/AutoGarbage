#ifndef GC_TESTS_USABILITY_TESTS_H
#define GC_TESTS_USABILITY_TESTS_H

#include "Test.h"
#include "gc.h"

namespace UsabilityTests
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

        void unsetA()
        {
            _a = nullptr;
        }
    };

    TEST(FieldNotEqualNullptrComparison)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> b(new B());

        if (b->_a != nullptr)
        {
            TEST_SUCCESS;
        }
        else
        {
            TEST_FAIL;
        }
    }

    TEST(FieldEqualNullptrComparison)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> b(new B());
        b->unsetA();

        if (b->_a == nullptr)
        {
            TEST_SUCCESS;
        }
        else
        {
            TEST_FAIL;
        }
    }

    TEST(StaticPtrNotEqualNullptrComparison)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> b(new B());

        if (b != nullptr)
        {
            TEST_SUCCESS;
        }
        else
        {
            TEST_FAIL;
        }
    }

    TEST(StaticPtrEqualNullptrComparison)
    {
        gc::init(4096, 25);

        gc::static_ptr<B> b(nullptr);

        if (b == nullptr)
        {
            TEST_SUCCESS;
        }
        else
        {
            TEST_FAIL;
        }
    }

    int run_tests()
    {
        using namespace UsabilityTests;

        RUN_TEST(FieldNotEqualNullptrComparison);
        RUN_TEST(FieldEqualNullptrComparison);
        RUN_TEST(StaticPtrNotEqualNullptrComparison);
        RUN_TEST(StaticPtrEqualNullptrComparison);

        TEST_SUCCESS;
    }
}

#endif

