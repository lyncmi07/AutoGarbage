#ifndef GC_LIB_FUNCTIONS_H
#define GC_LIB_FUNCTIONS_H

namespace gc
{
    void init_gc(size_t heap_size);
    void* malloc(size_t size);
}

#endif
