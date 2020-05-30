#ifndef GC_LIB_STATICS_H
#define GC_LIB_STATICS_H

#include <unordered_set>

namespace gc
{
    void init(size_t heap_size, unsigned int max_allocation_attempts_before_gc);
    void debug();
    void info();
}

#endif
