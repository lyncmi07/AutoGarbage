#ifndef GC_LIB_STATICS_H
#define GC_LIB_STATICS_H

#include <cstdlib>

namespace gc
{
    /**
     * Initiates the AutoGarbage library. This must be called before attempting to allocate any gc::objects.
     * @param    heap_size                             The size in bytes of the heap.
     * @param    max_allocation_attempts_before_gc     The maximum number of times an allocation attempt is made for an object before the system gives up and collects garbage.
     */
    void init(size_t heap_size, unsigned int max_allocation_attempts_before_gc);

    /**
     * Print verbose debug information to the console about the current state of the AutoGarbage system.
     */
    void debug();

    /**
     * Print information to the console about the current state of the AutoGarbage system.
     */
    void info();
}

#endif
