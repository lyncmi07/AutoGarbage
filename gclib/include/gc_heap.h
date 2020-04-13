#ifndef GC_LIB_HEAP_H
#define GC_LIB_HEAP_H

#include "gc_object.h"
#include "gc_free_cell.h"

namespace gc
{ namespace heap
{
    class heap_struct
    {
        private:
            static gc::heap::heap_struct* INSTANCE;

            size_t _heap_size;
            void* _heap_space;

            gc::object* _bottom;
            gc::object* _bottom_initial;

            gc::object* _top;

            gc::object* _scan;
            gc::object* _scan_initial;

            gc::heap::free_cell* _free;

            heap_struct(size_t heap_size):
                _heap_size(heap_size),
                _heap_space((void*) new char[_heap_size]),
                _bottom(nullptr),
                _bottom_initial(nullptr),
                _top(nullptr),
                _scan(nullptr),
                _scan_initial(nullptr)
            {
                gc::heap::free_cell* initial_free_cell = (gc::heap::free_cell*) _heap_space;
                (*initial_free_cell) = gc::heap::free_cell(nullptr, heap_size);

                _free = initial_free_cell;
            }

        public:
            static void init_gc(size_t heap_size)
            {
                gc::heap::heap_struct::INSTANCE = new heap_struct(heap_size);
            }

            inline static gc::heap::heap_struct* get()
            {
                return INSTANCE;
            }

            void* malloc(size_t size)
            {
                gc::heap::free_cell* next_free_cell = _free;
                gc::heap::free_cell* current_largest_cell = _free;

                while(next_free_cell != nullptr)
                {
                    if (next_free_cell->size() > size)
                    {
                        gc::heap::free_cell* resized_cell = next_free_cell->resize(size);
                        if (next_free_cell == current_largest_cell || resized_cell->size() >= current_largest_cell->size())
                        {
                            _free = resized_cell;
                        }
                        else
                        {
                            _free = current_largest_cell;
                        }

                        return next_free_cell;
                    }
                    else if (next_free_cell ->size() == size)
                    {
                        _free = current_largest_cell;
                        return next_free_cell;
                    }
                    else if(next_free_cell-> size() > current_largest_cell->size())
                    {
                        current_largest_cell = next_free_cell;
                    }

                    next_free_cell = next_free_cell->fwd_free_cell();
                }

                throw std::bad_alloc(); //TODO: Could improve by performing gc and trying to allocate again
            }

            inline gc::object* bottom()
            {
                return _bottom;
            }

            inline gc::object* top()
            {
                return _top;
            }

            inline gc::object* scan()
            {
                return _scan;
            }
            inline void set_scan(gc::object* new_scan)
            {
                _scan = new_scan;
            }

            inline gc::heap::free_cell* free()
            {
                return _free;
            }
    };
}}

#endif
