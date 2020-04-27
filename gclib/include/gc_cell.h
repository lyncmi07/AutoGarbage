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
            void* _actual_position;
        public:
            void unlink();
            void fwd_link(gc::heap::cell* next_cell);
            void back_link(gc::heap::cell* prev_cell);

            cell(size_t size);
            cell(gc::heap::cell* back_cell, gc::heap::cell* fwd_cell, size_t size);

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

            inline void* actual_position()
            {
                return _actual_position;
            }

            void set_actual_position(void* actual_position);
    };
}}

#endif
