#include "gc_cell.h"
#include "gc_heap.h"

#include <exception>

gc::heap::cell::cell(size_t size, bool using_vtable_offset):
    _size(size),
    _using_vtable_offset(using_vtable_offset)
{
}

gc::heap::cell::cell(gc::heap::cell* back_treadmill, gc::heap::cell* fwd_treadmill, size_t size, bool using_vtable_offset):
    _back_treadmill(back_treadmill),
    _fwd_treadmill(fwd_treadmill),
    _size(size),
    _using_vtable_offset(using_vtable_offset)
{
}

void gc::heap::cell::unlink_treadmill()
{
    if (_back_treadmill != nullptr) _back_treadmill->_fwd_treadmill = _fwd_treadmill;
    if (_fwd_treadmill != nullptr) _fwd_treadmill->_back_treadmill = _back_treadmill;
}

void gc::heap::cell::fwd_link_treadmill(gc::heap::cell* next_cell)
{
    if (next_cell == this) throw std::exception();
    _fwd_treadmill = next_cell;
    if (_fwd_treadmill != nullptr) _fwd_treadmill->_back_treadmill = this;
}

void gc::heap::cell::back_link_treadmill(gc::heap::cell* prev_cell)
{
    if (prev_cell == this) throw std::exception();
    _back_treadmill = prev_cell;
    if (_back_treadmill != nullptr) _back_treadmill->_fwd_treadmill = this;
}

void gc::heap::cell::unlink_location()
{
    if (_back_location != nullptr) _back_location->_fwd_location = _fwd_location;
    if (_fwd_location != nullptr) _fwd_location->_back_location = _back_location;
}

void gc::heap::cell::fwd_link_location(gc::heap::cell* next_cell)
{
    if (next_cell == this) throw std::exception();
    _fwd_location = next_cell;
    if (_fwd_location != nullptr) _fwd_location->_back_location = this;
}

void gc::heap::cell::back_link_location(gc::heap::cell* prev_cell)
{
    if (prev_cell == this) throw std::exception();
    _back_location = prev_cell;
    if (_back_location != nullptr) _back_location->_fwd_location = this;
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

    if (!mergable_with_fwd_treadmill() && new_size < sizeof(gc::heap::cell))
    {
        //not large enough to support a cell, moving to fragment memory
        gc::heap::heap_struct::get()->add_fragment_memory(new_cell_position, new_size);

        unlink_treadmill();
        return nullptr;
    }

    if (mergable_with_fwd_treadmill())
    {
        //merge with next cell
        (*new_cell_position) = cell(back_treadmill(), fwd_treadmill()->fwd_treadmill(), new_size + fwd_treadmill()->size(), false);
    }
    else
    {
        (*new_cell_position) = cell(back_treadmill(), fwd_treadmill(), new_size, false);
    }

    new_cell_position->back_treadmill()->fwd_link_treadmill(new_cell_position);
    new_cell_position->fwd_treadmill()->back_link_treadmill(new_cell_position);

    return new_cell_position;
}

bool gc::heap::cell::mergable_with_back_treadmill()
{
    return back_treadmill()->mergable_with_fwd_treadmill();
}

bool gc::heap::cell::mergable_with_fwd_treadmill()
{
    void* contiguous_position = (void*)((char*)actual_position()) + size();

    return contiguous_position == (void*)((char*)fwd_treadmill()->actual_position());
}

void gc::heap::cell::merge_with_fwd_treadmill()
{
    size_t fwd_cell_size = fwd_treadmill()->size();
    fwd_link_treadmill(fwd_treadmill()->fwd_treadmill());
    _size += fwd_cell_size;
}

bool gc::heap::cell::mergable_with_back_location()
{
    return back_location()->mergable_with_fwd_location();
}

bool gc::heap::cell::mergable_with_fwd_location()
{
    void* contiguous_position = (void*)((char*)actual_position()) + size();

    return contiguous_position == (void*)((char*)fwd_location()->actual_position());
}

void gc::heap::cell::merge_with_fwd_location()
{
    size_t fwd_cell_size = fwd_location()->size();
    fwd_link_location(fwd_location()->fwd_location());
    _size += fwd_cell_size;
}
