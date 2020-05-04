#ifndef GC_LIB_CELL_H
#define GC_LIB_CELL_H

#include <cstdlib>

namespace gc
{ namespace heap
{
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
        public:
            void unlink_treadmill();
            void fwd_link_treadmill(gc::heap::cell* next_cell);
            void back_link_treadmill(gc::heap::cell* prev_cell);

            void unlink_location();
            void fwd_link_location(gc::heap::cell* next_cell);
            void back_link_location(gc::heap::cell* prev_cell);

            cell(size_t size, bool using_vtable_offset, char iteration);
            cell(gc::heap::cell* back_treadmill, gc::heap::cell* fwd_treadmill, size_t size, bool using_vtable_offset, char iteration);

            inline size_t size();

            inline gc::heap::cell* back_treadmill();
            inline gc::heap::cell* fwd_treadmill();

            inline gc::heap::cell* back_location();
            inline gc::heap::cell* fwd_location();

            void* actual_position();
            gc::heap::cell* resize(size_t size_decrease);

            bool mergable_with_back_treadmill();
            bool mergable_with_fwd_treadmill();
            //Makes the assumption that the back cell has an address which is larger than this
            void merge_with_fwd_treadmill();

            bool mergable_with_back_location();
            bool mergable_with_fwd_location();
            void merge_with_fwd_location();

            bool garunteed_free();

            bool iteration_stale();
            void update_iteration();
    };
}}

size_t gc::heap::cell::size()
{
    return _size;
}

gc::heap::cell* gc::heap::cell::back_treadmill()
{
    return _back_treadmill;
}

gc::heap::cell* gc::heap::cell::fwd_treadmill()
{
    return _fwd_treadmill;
}

gc::heap::cell* gc::heap::cell::back_location()
{
    return _back_location;
}

gc::heap::cell* gc::heap::cell::fwd_location()
{
    return _fwd_location;
}

#endif
