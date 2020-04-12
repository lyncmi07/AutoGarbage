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
            free_cell(gc::heap::free_cell* fwd_cell, size_t size);

            gc::heap::free_cell* resize(size_t size_decrease);

            inline gc::heap::free_cell* fwd_free_cell()
            {
                return (gc::heap::free_cell*) fwd_cell();
            }
    };
}}

#endif
