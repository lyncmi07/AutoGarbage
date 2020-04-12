#ifndef GC_LIB_CELL_H
#define GC_LIB_CELL_H

namespace gc
{ namespace heap
{
    class cell
    {
        private:
            gc::heap::cell* _fwd_cell;
            size_t _size;
        public:
            cell(gc::heap::cell* fwd_cell, size_t size):
                _fwd_cell(fwd_cell),
                _size(size)
            {
            }


            inline size_t size()
            {
                return _size;
            }

            inline gc::heap::cell* fwd_cell()
            {
                return _fwd_cell;
            }

    };
}}

#endif
