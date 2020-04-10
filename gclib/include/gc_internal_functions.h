#ifndef GC_LIB_INTERNAL_FUNCTIONS_H
#define GC_LIB_INTERNAL_FUNCTIONS_H

namespace gc
{
    template<class T> class static_ptr;

    void add_static(void* sptr);
    void remove_static(void* sptr);

    void print_gc_debug();
}

#endif

