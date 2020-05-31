#ifndef GC_TEST_ACCESS_H
#define GC_TEST_ACCESS_H

#include "gc_field.h"
#include "gc_static_ptr.h"

namespace gc
{
    class object;
    namespace heap {
        class cell;
    }
}

namespace gc_test_access
{
    void collect_garbage();

    gc::object* bottom();
    gc::object* top();
    gc::object* scan();
    gc::heap::cell* free();

    gc::heap::cell* back_treadmill(const gc::heap::cell* cell);
    gc::heap::cell* fwd_treadmill(const gc::heap::cell* cell);

    template<class T> T* debug_object(const gc::field<T>& field)
    {
        return field._object;
    }

    template<class T> T* debug_object(const gc::static_ptr<T>& static_ptr)
    {
        return static_ptr._object;
    }
}

#endif
