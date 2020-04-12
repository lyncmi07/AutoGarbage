#include <new>

#include "gc_free_cell.h"

gc::heap::free_cell::free_cell(gc::heap::free_cell* fwd_cell, size_t size):
    cell(fwd_cell, size)
{
}

gc::heap::free_cell* gc::heap::free_cell::resize(size_t size_decrease)
{
    if (size_decrease >= size()) throw std::bad_alloc();

    size_t new_size = size() - size_decrease;
    gc::heap::free_cell* new_cell_position = (this + (size_decrease));

    (*new_cell_position) = free_cell(fwd_free_cell(), new_size);

    return new_cell_position;
}


