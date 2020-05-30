#ifndef GC_TEST_ACCESS_H
#define GC_TEST_ACCESS_H

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
}

#endif
