#include <unordered_set>

#include "gc_static_ptr.h"

namespace gc
{
    class object;
    namespace heap
    {
        class free_cell;
    }

    char* _heap_space = nullptr;
    size_t next_ptr_offset = 0;;

    std::unordered_set<gc::object*> _white_objects;
    std::unordered_set<gc::object*> _black_objects;

    unsigned int _current_grey_objects = 0;

    gc::static_ptr<bool>* _static_objects_start_ptr = nullptr;

    namespace heap
    {
        /*gc::object* _bottom = nullptr;
        gc::object* _top = nullptr;
        gc::object* _scan = nullptr;
        gc::heap::free_cell* _free = nullptr;*/

        bool _odd_iteration = true;
    }
}
