#include "gc_test_access.h"

#include "gc_heap.h"
#include "gc_cell.h"
#include "gc_object.h"
#include "gc_field.h"
#include "gc_static_ptr.h"

void gc_test_access::collect_garbage()
{
    gc::heap::heap_struct::get()->collect_garbage();
}

gc::object* gc_test_access::bottom()
{
    gc::heap::heap_struct::get()->bottom();
}
gc::object* gc_test_access::top()
{
    gc::heap::heap_struct::get()->top();
}
gc::object* gc_test_access::scan()
{
    gc::heap::heap_struct::get()->scan();
}
gc::heap::cell* gc_test_access::free()
{
    gc::heap::heap_struct::get()->free();
}

gc::heap::cell* gc_test_access::back_treadmill(const gc::heap::cell* cell)
{
    cell->back_treadmill();
}
gc::heap::cell* gc_test_access::fwd_treadmill(const gc::heap::cell* cell)
{
    cell->fwd_treadmill();
}
