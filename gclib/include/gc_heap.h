#ifndef GC_LIB_HEAP_H
#define GC_LIB_HEAP_H

#include <cstdlib>

namespace gc
{
    class object;
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

            gc::object* _bottom;
            gc::object* _bottom_initial;

            gc::object* _top;

            gc::object* _scan;
            gc::object* _scan_initial;

            gc::heap::free_cell* _free;

            gc::static_ptr<bool>* _static_objects_start_ptr;

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

            inline gc::static_ptr<bool>* static_objects_start_ptr()
            {
                return _static_objects_start_ptr;
            }

            inline bool odd_iteration()
            {
                return _odd_iteration;
            }

            void add_static(void* sptr);
            void remove_static(void* sptr);

            void print_gc_debug();
    };
}}

#endif
