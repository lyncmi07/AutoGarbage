#include "gc_cell.h"


gc::heap::cell::cell(gc::heap::cell* back_cell, gc::heap::cell* fwd_cell, size_t size):
    _back_cell(back_cell),
    _fwd_cell(fwd_cell),
    _size(size)
{
}

void gc::heap::cell::unlink()
{
    if (_back_cell != nullptr) _back_cell->_fwd_cell = _fwd_cell;
    if (_fwd_cell != nullptr) _fwd_cell->_back_cell = _back_cell;
}

void gc::heap::cell::fwd_link(gc::heap::cell* next_cell)
{
    _fwd_cell = next_cell;
}

void gc::heap::cell::back_link(gc::heap::cell* prev_cell)
{
    _back_cell = prev_cell;
}
