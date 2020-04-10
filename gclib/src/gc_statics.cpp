#include <unordered_set>

#include "gc_static_ptr.h"

namespace gc
{
    class object;

    size_t _heap_size = 0;
    char* _heap_space = nullptr;
    size_t next_ptr_offset = 0;;

    std::unordered_set<gc::object*> _white_objects;
    std::unordered_set<gc::object*> _black_objects;

    unsigned int _current_grey_objects = 0;

    gc::static_ptr<bool>* static_objects_start_ptr;
}
