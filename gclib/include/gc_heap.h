#ifndef GC_LIB_HEAP_H
#define GC_LIB_HEAP_H

#include <cstdlib>

#include "gc_cell.h"
#include "gc_object.h"

namespace gc
{
    template<class T> class static_ptr;
namespace heap
{
    class free_cell;

    class heap_struct
    {
        private:
            static gc::heap::heap_struct* INSTANCE;

            size_t _heap_size;
            void* _heap_space;

            gc::heap::cell *const _bottom;

            gc::heap::cell *const _top;

            gc::heap::cell *const _scan;

            gc::heap::cell *const _free;

            gc::static_ptr<gc::object>* _static_objects_start_ptr;

            bool _odd_iteration;

            heap_struct(size_t heap_size);

            void print_static_objects_list();
            void print_heap_pointers();
        public:
            void* malloc(size_t size);

            static void init_gc(size_t heap_size)
            {
                gc::heap::heap_struct::INSTANCE = new heap_struct(heap_size);
            }

            inline static gc::heap::heap_struct* get()
            {
                return INSTANCE;
            }

            inline gc::object* bottom()
            {
                return (gc::object*) _bottom->fwd_cell();
            }

            inline gc::object* top()
            {
                return (gc::object*) _top->fwd_cell();
            }

            inline gc::object* scan()
            {
                return (gc::object*) _scan->fwd_cell();
            }
            inline void set_scan(gc::object* new_scan)
            {
                _scan->fwd_link(new_scan);
            }

            inline gc::heap::free_cell* free()
            {
                return (gc::heap::free_cell*) _free->fwd_cell();
            }

            inline gc::static_ptr<gc::object>* static_objects_start_ptr()
            {
                return _static_objects_start_ptr;
            }

            inline bool odd_iteration()
            {
                return _odd_iteration;
            }

            void add_static(void* sptr);
            void remove_static(void* sptr);

            void link_bottom(gc::object* obj);
            void link_top(gc::object* obj);
            void link_scan(gc::object* obj);
            void link_free(gc::heap::free_cell* obj);

            void replace_free_start(gc::heap::free_cell* replacement);

            void print_gc_debug();
    };
}}

#endif
