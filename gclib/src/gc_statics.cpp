#include <unordered_set>

#include "gc_static_ptr.h"

namespace gc
{
    class object;
    namespace heap
    {
        class free_cell;
    }

    size_t next_ptr_offset = 0;

    unsigned int _current_grey_objects = 0;

    namespace heap
    {
        bool _odd_iteration = true;
    }
}
