#include "gc_test_access.h"

#include "gc_heap.h"
#include "gc_cell.h"
#include "gc_object.h"

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