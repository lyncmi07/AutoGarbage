#ifndef GC_TESTS_ARRAY_TESTS_H
#define GC_TESTS_ARRAY_TESTS_H

#include "Test.h"

#include "gc.h"

namespace ArrayTests
{

struct A : public gc::object
{
    END_GC_FIELDS;
    int a;

    A(int _a):
        a(_a)
    {
    }
};

TEST(ReferencedArrayIsNotCollected)
{
    gc::init(4096, 25);

    gc::static_ptr<gc::array<A>> arr(new gc::array<A>(3));

    gc_test_access::collect_garbage();
    ASSERT_TOP(gc_test_access::debug_object(arr));

    TEST_SUCCESS;
}

TEST(ObjectsInArrayNotCollected)
{
    gc::init(4096, 25);

    A* a0ptr;
    A* a1ptr;
    A* a2ptr;
    gc::static_ptr<gc::array<A>> arr(new gc::array<A>(3));
    {
        gc::static_ptr<A> a0(new A(0));
        a0ptr = gc_test_access::debug_object(a0);
        gc::static_ptr<A> a1(new A(1));
        a1ptr = gc_test_access::debug_object(a1);
        gc::static_ptr<A> a2(new A(2));
        a2ptr = gc_test_access::debug_object(a2);

        arr[0] = a0;
        arr[1] = a1;
        arr[2] = a2;
    }

    gc_test_access::collect_garbage();
    gc_test_access::collect_garbage();

    // The objects are not referenced in static pointers so should appear in the scan list after garbage collection
    ASSERT_SCAN(a0ptr);
    ASSERT_SCAN(a1ptr);
    ASSERT_SCAN(a2ptr);
    
    TEST_SUCCESS;
}

TEST(IndexSpacedObjectsInArrayNotCollected)
{
    gc::init(4096, 25);

    A* a0ptr;
    A* a2ptr;
    gc::static_ptr<gc::array<A>> arr(new gc::array<A>(3));
    {
        gc::static_ptr<A> a0(new A(0));
        a0ptr = gc_test_access::debug_object(a0);
        gc::static_ptr<A> a2(new A(2));
        a2ptr = gc_test_access::debug_object(a2);

        arr[0] = a0;
        arr[2] = a2;
    }

    gc_test_access::collect_garbage();
    gc_test_access::collect_garbage();

    // The objects are not referenced in static pointers so should appear in the scan list after garbage collection
    ASSERT_SCAN(a0ptr);
    ASSERT_SCAN(a2ptr);
    
    TEST_SUCCESS;
}

TEST(UnassignedElementCollectedNewElementNotCollected)
{
    gc::init(4096, 25);

    A* originalObj;
    A* newObj;
    gc::static_ptr<gc::array<A>> arr(new gc::array<A>(3));
    {
        gc::static_ptr<A> a0(new A(0));
        originalObj = gc_test_access::debug_object(a0);

        arr[0] = a0;
    }

    gc_test_access::collect_garbage();

    {
        gc::static_ptr<A> a1(new A(1));
        newObj = gc_test_access::debug_object(a1);

        arr[0] = a1;
    }

    // On initial collect, the original object would have been marked before it became unreferenced
    gc_test_access::collect_garbage();
    ASSERT_SCAN(newObj);
    ASSERT_SCAN(originalObj);

    gc_test_access::collect_garbage();
    ASSERT_SCAN(newObj);
    ASSERT_FREE(originalObj);

    TEST_SUCCESS;
}

TEST(MultiDimentionalElementsNotCollected)
{
    gc::init(4096, 25);

    gc::static_ptr<gc::array<gc::array<A>>> arr(new gc::array<gc::array<A>>(2));

    arr[0] = new gc::array<A>(2);
    arr[1] = new gc::array<A>(2);

    A* a00ptr;
    A* a01ptr;
    A* a10ptr;
    A* a11ptr;
    {
        gc::static_ptr<A> a00 = new A(0);
        a00ptr = gc_test_access::debug_object(a00);
        gc::static_ptr<A> a01 = new A(1);
        a01ptr = gc_test_access::debug_object(a01);
        gc::static_ptr<A> a10 = new A(2);
        a10ptr = gc_test_access::debug_object(a10);
        gc::static_ptr<A> a11 = new A(3);
        a11ptr = gc_test_access::debug_object(a11);

        arr[0][0] = a00;
        arr[0][1] = a01;
        arr[1][0] = a10;
        arr[1][1] = a11;
    }

    gc_test_access::collect_garbage();
    gc_test_access::collect_garbage();

    ASSERT_SCAN(a00ptr);
    ASSERT_SCAN(a01ptr);
    ASSERT_SCAN(a10ptr);
    ASSERT_SCAN(a11ptr);
    
    TEST_SUCCESS;
}

int run_tests()
{
    using namespace ArrayTests;

    RUN_TEST(ReferencedArrayIsNotCollected);
    RUN_TEST(ObjectsInArrayNotCollected);
    RUN_TEST(IndexSpacedObjectsInArrayNotCollected);
    RUN_TEST(UnassignedElementCollectedNewElementNotCollected);
    RUN_TEST(MultiDimentionalElementsNotCollected);
}
}
#endif
