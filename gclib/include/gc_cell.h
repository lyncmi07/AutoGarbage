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
            size_t _size;

            bool _using_vtable_offset;
        public:
            void unlink_treadmill();
            void fwd_link_treadmill(gc::heap::cell* next_cell);
            void back_link_treadmill(gc::heap::cell* prev_cell);

            cell(size_t size, bool using_vtable_offset);
            cell(gc::heap::cell* back_treadmill, gc::heap::cell* fwd_treadmill, size_t size, bool using_vtable_offset);

            inline size_t size()
            {
                return _size;
            }

            inline gc::heap::cell* back_treadmill()
            {
                return _back_treadmill;
            }
            
            inline gc::heap::cell* fwd_treadmill()
            {
                return _fwd_treadmill;
            }

            void* actual_position();
            gc::heap::cell* resize(size_t size_decrease);

            bool mergable_with_back_treadmill();
            bool mergable_with_fwd_treadmill();

            //Makes the assumption that the back cell has an address which is larger than this
            void merge_with_fwd_treadmill();
    };
}}

#endif
