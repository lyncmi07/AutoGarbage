#ifndef GC_LIB_HEAP_H
#define GC_LIB_HEAP_H

#include <cstdlib>
#include <vector>

#include "gc_test_access.h"

namespace gc
{
    template<class T> class field;
    template<class T> class static_ptr;
    template<class T> class array;
    template<class T> class dyn_array_impl;
    class object;
    void init(size_t heap_size, unsigned int max_allocation_attempts_before_gc);
    void debug();
    void info();

namespace heap
{

    struct fragment_memory
    {
        void* fragment_position;
        size_t size;

        fragment_memory(void* _fragment_position, size_t _size);
    };

    class cell;

    class heap_struct
    {
        private:
            static gc::heap::heap_struct* INSTANCE;

            // config values
            size_t _heap_size;
            unsigned int _max_allocation_attempts_before_gc;

            void* _heap_space;
            void* _end_gc_fields_magic_ptr;

            unsigned long _total_bytes_allocated;

            unsigned int _garbage_collection_cycle;

            gc::heap::cell *const _bottom;
            gc::heap::cell *const _top;
            gc::heap::cell *const _scan;
            gc::heap::cell *const _free;

            gc::static_ptr<gc::object>* _static_objects_start_ptr;
            gc::object* _initialization_objects_start_ptr;

            std::vector<gc::heap::fragment_memory*> _fragment_memory_list;
            unsigned int _fragment_size;

            char _gc_iteration;

            heap_struct(size_t heap_size, unsigned int max_allocation_attempts_before_gc);
            ~heap_struct();

            static void init_gc(
                    size_t heap_size,
                    unsigned int max_allocation_attempts_before_gc)
            {
                if (gc::heap::heap_struct::INSTANCE != nullptr) delete gc::heap::heap_struct::INSTANCE;
                gc::heap::heap_struct::INSTANCE = new heap_struct(heap_size, max_allocation_attempts_before_gc);
            }

            void flip_list(); 
            void ungrey_all();
            void grey_static_ptrs();

            void print_initialization_objects_list();
            void print_static_objects_list();
            void print_heap_pointers();

            void print_gc_debug();
            void print_gc_info();

            gc::heap::cell* attempt_allocate(size_t size);

            void* malloc(size_t size);

            inline gc::static_ptr<gc::object>* static_objects_start_ptr();
            inline gc::object* initialization_objects_start_ptr();

            gc::object* bottom();
            gc::object* top();
            gc::object* scan();
            void set_scan(gc::object* new_scan);
            gc::heap::cell* free();

            inline char gc_iteration();

            void add_static(void* sptr);
            void remove_static(void* sptr);

            void make_black(gc::object* obj);
            void make_grey(gc::object* obj);
            void make_ecru(gc::object* obj);
            void make_white(gc::heap::cell* obj);


            void replace_free_start(gc::heap::cell* replacement);

            //gc memory once grey objects at zero
            void flip();

            //Ensure there are no grey objects left and flip heap
            void collect_garbage();

            void add_to_initialization_list(gc::object* object);
            void remove_from_initialization_list(gc::object* object);

            void add_fragment_memory(void* position, size_t size);


            //library friend functions
            friend void gc::init(size_t heap_size, unsigned int max_allocation_attempts_before_gc);
            friend void gc::debug();
            friend void gc::info();

            //library friend classes
            friend class gc::object;
            friend class gc::heap::cell;
            template<class A> friend class gc::array;
            template<class D> friend class gc::dyn_array_impl;
            template<class F> friend class gc::field;
            template<class S> friend class gc::static_ptr;

            //test friends
            friend void gc_test_access::collect_garbage();
            friend gc::object* gc_test_access::bottom();
            friend gc::object* gc_test_access::top();
            friend gc::object* gc_test_access::scan();
            friend gc::heap::cell* gc_test_access::free();

        public:
            inline void* end_gc_fields_magic_ptr();
            inline static gc::heap::heap_struct* get()
            {
                return INSTANCE;
            }

    };
}}


gc::static_ptr<gc::object>* gc::heap::heap_struct::static_objects_start_ptr()
{
    return _static_objects_start_ptr;
}

gc::object* gc::heap::heap_struct::initialization_objects_start_ptr()
{
    return _initialization_objects_start_ptr;
}

void* gc::heap::heap_struct::end_gc_fields_magic_ptr()
{
    return _end_gc_fields_magic_ptr;
}

char gc::heap::heap_struct::gc_iteration()
{
    return _gc_iteration;
}
#endif
