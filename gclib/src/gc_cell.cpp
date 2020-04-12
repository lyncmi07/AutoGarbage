#include "gc_cell.h"


gc::heap::cell::cell(gc::heap::cell* fwd_cell, size_t size):
    _fwd_cell(fwd_cell),
    _size(size)
{
}


