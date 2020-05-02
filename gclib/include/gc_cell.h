#ifndef GC_LIB_CELL_H
#define GC_LIB_CELL_H

#include <cstdlib>

namespace gc
{ namespace heap
{
    class cell
    {
        private:
            gc::heap::cell* _back_cell;
            gc::heap::cell* _fwd_cell;
            size_t _size;

            bool _using_vtable_offset;
        public:
            void unlink();
            void fwd_link(gc::heap::cell* next_cell);
            void back_link(gc::heap::cell* prev_cell);

            cell(size_t size, bool using_vtable_offset);
            cell(gc::heap::cell* back_cell, gc::heap::cell* fwd_cell, size_t size, bool using_vtable_offset);

            inline size_t size()
            {
                return _size;
            }

            inline gc::heap::cell* back_cell()
            {
                return _back_cell;
            }
            
            inline gc::heap::cell* fwd_cell()
            {
                return _fwd_cell;
            }

            void* actual_position();
            gc::heap::cell* resize(size_t size_decrease);

            bool mergable_with_back_cell();
            bool mergable_with_fwd_cell();

            //Makes the assumption that the back cell has an address which is larger than this
            void merge_with_back_cell();
    };
}}

#endif
