#ifndef GC_LIB_STATICS_H
#define GC_LIB_STATICS_H

#include <unordered_set>

namespace gc
{
    class object;
    namespace heap
    {
        class free_cell;
    }

    extern unsigned int _current_grey_objects;
}

#endif
