#ifndef GC_LIB_STATICS_H
#define GC_LIB_STATICS_H

#include <unordered_set>

#include "gc_static_ptr.h"

namespace gc
{
    class object;
    namespace heap
    {
        class free_cell;
    }

    extern char* _heap_space;

    extern std::unordered_set<gc::object*> _white_objects;
    extern std::unordered_set<gc::object*> _black_objects;

    extern unsigned int _current_grey_objects;

    extern gc::static_ptr<bool>* _static_objects_start_ptr;

    namespace heap
    {
        extern gc::object* _bottom;
        extern gc::object* _top;
        extern gc::object* _scan;
        extern gc::heap::free_cell* _free;

        extern bool _odd_iteration;
    }
}

#endif
