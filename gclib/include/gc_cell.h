#ifndef GC_LIB_CELL_H
#define GC_LIB_CELL_H

#include <cstdlib>
#include "gc_test_access.h"

namespace gc
{ 

    class object;

namespace heap
{
    class heap_struct;

    class cell
    {
        private:
            gc::heap::cell* _back_treadmill;
            gc::heap::cell* _fwd_treadmill;

            gc::heap::cell* _back_location;
            gc::heap::cell* _fwd_location;

            size_t _size;
            char _iteration;

            bool _using_vtable_offset;

            void unlink_treadmill();
            void fwd_link_treadmill(gc::heap::cell* next_cell);
            void back_link_treadmill(gc::heap::cell* prev_cell);

            void unlink_location();
            void fwd_link_location(gc::heap::cell* next_cell);
            void back_link_location(gc::heap::cell* prev_cell);

            cell(bool using_vtable_offset, char iteration);
            cell(gc::heap::cell* back_treadmill, gc::heap::cell* fwd_treadmill, size_t size, bool using_vtable_offset, char iteration);

            inline size_t size() const;
            inline void set_size(size_t size);

            inline gc::heap::cell* back_treadmill() const;
            inline gc::heap::cell* fwd_treadmill() const;

            inline gc::heap::cell* back_location() const;
            inline gc::heap::cell* fwd_location() const;

            void* actual_position() const;
            gc::heap::cell* resize(size_t size_decrease);

            bool mergable_with_back_location() const;
            bool mergable_with_fwd_location() const;
            void merge_with_fwd_location();

            bool garunteed_free() const;

            bool iteration_stale() const;
            void update_iteration();

            friend class gc::object;
            friend class gc::heap::heap_struct;
            friend gc::heap::cell* gc_test_access::back_treadmill(const gc::heap::cell* cell);
            friend gc::heap::cell* gc_test_access::fwd_treadmill(const gc::heap::cell* cell);
    };
}}

size_t gc::heap::cell::size() const
{
    return _size;
}

void gc::heap::cell::set_size(size_t size)
{
    _size = size;
}

gc::heap::cell* gc::heap::cell::back_treadmill() const
{
    return _back_treadmill;
}

gc::heap::cell* gc::heap::cell::fwd_treadmill() const
{
    return _fwd_treadmill;
}

gc::heap::cell* gc::heap::cell::back_location() const
{
    return _back_location;
}

gc::heap::cell* gc::heap::cell::fwd_location() const
{
    return _fwd_location;
}

#endif
