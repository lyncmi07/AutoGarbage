#include "gc_statics.h"
#include "gc_free_cell.h"

namespace gc
{
    /*void init_gc(size_t heap_size)
    {
        gc::_heap_space = new char[heap_size];

        gc::heap::free_cell* initial_free_cell = (gc::heap::free_cell*) _heap_space;
        (*initial_free_cell) = gc::heap::free_cell(nullptr, heap_size);
        gc::heap::_free = initial_free_cell;
    }*/

    /*void* malloc(size_t size)
    {
        gc::heap::free_cell* next_free_cell = gc::heap::_free;
        gc::heap::free_cell* current_largest_cell = gc::heap::_free;

        while(next_free_cell != nullptr)
        {
            if (next_free_cell->size() > size)
            {
                gc::heap::free_cell* resized_cell = next_free_cell->resize(size);
                if (next_free_cell == current_largest_cell || resized_cell->size() >= current_largest_cell->size())
                {
                    gc::heap::_free = resized_cell;
                }
                else
                {
                    gc::heap::_free = current_largest_cell;
                }

                return next_free_cell;
            }
            else if (next_free_cell->size() == size)
            {
                gc::heap::_free = current_largest_cell;
                return next_free_cell;
            }
            else if(next_free_cell->size() > current_largest_cell->size())
            {
                current_largest_cell = next_free_cell;
            }

            next_free_cell = next_free_cell->fwd_free_cell();
        }

        throw std::bad_alloc(); //TODO: Could improve by performing gc and trying to allocate again
    }*/
}
