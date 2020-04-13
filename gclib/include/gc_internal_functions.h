#ifndef GC_LIB_INTERNAL_FUNCTIONS_H
#define GC_LIB_INTERNAL_FUNCTIONS_H

namespace gc
{
    void add_static(void* sptr);
    void remove_static(void* sptr);

    void flip();

    void print_gc_debug();
}

#endif

