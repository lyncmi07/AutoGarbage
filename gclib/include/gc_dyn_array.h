#ifndef GC_LIB_DYN_ARRAY_H
#define GC_LIB_DYN_ARRAY_H

#include "gc_heap.h"
#include "gc_dyn_array_impl.h"

namespace gc
{
    template<class T>
    class dyn_array : public gc::object
    {
        private:
            gc::field<gc::dyn_array_impl<T>> _internal_array;
            END_GC_FIELDS;
        public:
            dyn_array(size_t array_size):
                _internal_array(nullptr)
            {
                gc::dyn_array_impl<T>* _array_position =
                    (gc::dyn_array_impl<T>*) gc::heap::heap_struct::get()->malloc(
                            sizeof(gc::object)
                            + (sizeof(gc::field<T>) * array_size)
                            + (sizeof(void*))
                        );
                _array_position = new (_array_position) gc::dyn_array_impl<T>(array_size);


                _internal_array = _array_position;
            }

            gc::field<T>& operator[](int i)
            {
                gc_mark();
                return _internal_array[i];
            }
    };
}

#endif
