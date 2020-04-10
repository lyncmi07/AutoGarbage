#include "gc_internal_functions.h"
#include "gc_static_ptr.h"
#include "gc_statics.h"

void gc::add_static(void* sptr)
{
    gc::static_ptr<bool>* sptr_typed = (gc::static_ptr<bool>*) sptr;
    sptr_typed->_back_static_object = nullptr;
    sptr_typed->_fwd_static_object = static_objects_start_ptr;

    if(static_objects_start_ptr != nullptr)
    {
        static_objects_start_ptr->_back_static_object = sptr_typed;
    }
    static_objects_start_ptr = sptr_typed;
}
