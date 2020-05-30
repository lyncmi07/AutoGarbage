#ifndef GC_TEST_ACCESS_H
#define GC_TEST_ACCESS_H

#include "gc_cell.h"
#include "gc_object.h"

namespace gc_test_access
{
    void collect_garbage();

    gc::object* bottom();
    gc::object* top();
    gc::object* scan();
    gc::heap::cell* free();
}

#endif
