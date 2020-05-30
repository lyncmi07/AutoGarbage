#include "gc_statics.h"

#include <iostream>
#include "gc_heap.h"

void gc::init(size_t heap_size, unsigned int max_allocation_attempts_before_gc)
{
    gc::heap::heap_struct::init_gc(heap_size, max_allocation_attempts_before_gc);
}
void gc::debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
}

void gc::info()
{
    gc::heap::heap_struct::get()->print_gc_info();
}
