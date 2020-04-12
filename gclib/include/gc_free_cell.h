#ifndef GC_LIB_FREE_CELL_H
#define GC_LIB_FREE_CELL_H

#include "gc_cell.h"

namespace gc
{ namespace heap
{
    class free_cell : public gc::heap::cell
    {
        private:
        public:
            free_cell(gc::heap::free_cell* fwd_cell, size_t size):
                cell(fwd_cell, size)
            {
            }

            gc::heap::free_cell* resize(size_t size_decrease)
            {
                if (size_decrease >= size()) throw std::bad_alloc();

                size_t new_size = size() - size_decrease;
                gc::heap::free_cell* new_cell_position = (this + (size_decrease));

                (*new_cell_position) = free_cell(fwd_free_cell(), new_size);

                return new_cell_position;
            }

            inline gc::heap::free_cell* fwd_free_cell()
            {
                return (gc::heap::free_cell*) fwd_cell();
            }
    };
}}

#endif
