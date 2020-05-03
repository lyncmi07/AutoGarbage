#include "gc_cell.h"
#include "gc_heap.h"

#include <exception>

gc::heap::cell::cell(size_t size, bool using_vtable_offset):
    _size(size),
    _using_vtable_offset(using_vtable_offset)
{
}

gc::heap::cell::cell(gc::heap::cell* back_cell, gc::heap::cell* fwd_cell, size_t size, bool using_vtable_offset):
    _back_cell(back_cell),
    _fwd_cell(fwd_cell),
    _size(size),
    _using_vtable_offset(using_vtable_offset)
{
}

void gc::heap::cell::unlink()
{
    if (_back_cell != nullptr) _back_cell->_fwd_cell = _fwd_cell;
    if (_fwd_cell != nullptr) _fwd_cell->_back_cell = _back_cell;
}

void gc::heap::cell::fwd_link(gc::heap::cell* next_cell)
{
    if (next_cell == this) throw std::exception();
    _fwd_cell = next_cell;
    if (_fwd_cell != nullptr) _fwd_cell->_back_cell = this;
}

void gc::heap::cell::back_link(gc::heap::cell* prev_cell)
{
    if (prev_cell == this) throw std::exception();
    _back_cell = prev_cell;
    if (_back_cell != nullptr) _back_cell->_fwd_cell = this;
}

void* gc::heap::cell::actual_position()
{
    char* actual_position = (char*) this;
    if (_using_vtable_offset) actual_position -= 8;
    return actual_position;
} 


gc::heap::cell* gc::heap::cell::resize(size_t size_decrease)
{
    if (size_decrease >= size()) throw std::bad_alloc();

    size_t new_size = size() - size_decrease;
    gc::heap::cell* new_cell_position = (gc::heap::cell*) (((char*)this->actual_position()) + size_decrease);

    if (!mergable_with_fwd_cell() && new_size < sizeof(gc::heap::cell))
    {
        //not large enough to support a cell, moving to fragment memory
        gc::heap::heap_struct::get()->add_fragment_memory(new_cell_position, new_size);

        unlink();
        return nullptr;
    }

    if (mergable_with_fwd_cell())
    {
        //merge with next cell
        (*new_cell_position) = cell(back_cell(), fwd_cell()->fwd_cell(), new_size + fwd_cell()->size(), false);
    }
    else
    {
        (*new_cell_position) = cell(back_cell(), fwd_cell(), new_size, false);
    }

    new_cell_position->back_cell()->fwd_link(new_cell_position);
    new_cell_position->fwd_cell()->back_link(new_cell_position);

    return new_cell_position;
}

bool gc::heap::cell::mergable_with_back_cell()
{
    return back_cell()->mergable_with_back_cell();
}

bool gc::heap::cell::mergable_with_fwd_cell()
{
    void* contiguous_position = (void*)((char*)actual_position()) + size();

    return contiguous_position == (void*)((char*)fwd_cell()->actual_position());
}

void gc::heap::cell::merge_with_fwd_cell()
{
    size_t fwd_cell_size = fwd_cell()->size();
    fwd_link(fwd_cell()->fwd_cell());
    _size += fwd_cell_size;
}
