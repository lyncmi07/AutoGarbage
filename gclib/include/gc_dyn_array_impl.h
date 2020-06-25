#ifndef GC_LIB_DYN_ARRAY_IMPL_H
#define GC_LIB_DYN_ARRAY_IMPL_H

#include <cstring>
#include "gc_object.h"

namespace gc
{
    template<class T>
    class dyn_array_impl : public gc::object
    {
        private:
            gc::field<T> _elements; //This is not a real member, it has type gc::field<T> to keep the array size correct.

        public:
            dyn_array_impl(size_t array_size)
            {
                size_t end_position = sizeof(gc::field<T>) * array_size;
                memset(&_elements, 0, end_position);
                void** _magic_value_position = (void**) &((&_elements)[array_size]);
                *_magic_value_position = gc::heap::heap_struct::get()->end_gc_fields_magic_ptr();
            }

            gc::field<T>& operator[](int i)
            {
                gc_mark();
                return (&_elements)[i];
            }

            void* operator new(size_t size, void* position)
            {
                return position;
            }
    };
}

#endif
