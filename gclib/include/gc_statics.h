#ifndef GC_LIB_STATICS_H
#define GC_LIB_STATICS_H

#include <unordered_set>

namespace gc
{
    class object;

    extern size_t _heap_size;
    extern char* _heap_space;
    extern size_t next_ptr_offset;

    extern std::unordered_set<gc::object*> _white_objects;
    extern std::unordered_set<gc::object*> _black_objects;

    extern unsigned int _current_grey_objects;
}

#endif
