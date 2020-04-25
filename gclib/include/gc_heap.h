#ifndef GC_LIB_HEAP_H
#define GC_LIB_HEAP_H

#include <cstdlib>

namespace gc
{
    template<class T> class static_ptr;
    class object;

namespace heap
{
    class free_cell;
    class cell;

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

            gc::object* _initialization_objects_start_ptr;

            bool _odd_iteration;

            heap_struct(size_t heap_size);
            ~heap_struct();


            void flip_list(); 
            void ungrey_all();
            void grey_static_ptrs();

            void print_initialization_objects_list();
            void print_static_objects_list();
            void print_heap_pointers();

            void* attempt_allocate(size_t size);
        public:
            void* malloc(size_t size);

            static void init_gc(unsigned int heap_size)
            {
                if (gc::heap::heap_struct::INSTANCE != nullptr) delete gc::heap::heap_struct::INSTANCE;
                gc::heap::heap_struct::INSTANCE = new heap_struct(heap_size);
            }

            inline static gc::heap::heap_struct* get()
            {
                return INSTANCE;
            }

            inline gc::static_ptr<gc::object>* static_objects_start_ptr()
            {
                return _static_objects_start_ptr;
            }

            inline gc::object* initialization_objects_start_ptr()
            {
                return _initialization_objects_start_ptr;
            }

            gc::object* bottom();
            gc::object* top();
            gc::object* scan();
            void set_scan(gc::object* new_scan);
            gc::heap::free_cell* free();

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

            //gc memory once grey objects at zero
            void flip();

            //Ensure there are no grey objects left and flip heap
            void collect_garbage();

            void print_gc_debug();

            void add_to_initialization_list(gc::object* object);
            void remove_from_initialization_list(gc::object* object);
    };
}}

#endif
