#ifndef GC_LIB_ARRAY_H
#define GC_LIB_ARRAY_H

#include "gc_heap.h"
#include "gc_dyn_array_impl.h"
#include "gc_object.h"

namespace gc
{
    /*!
     * Creates an array of objects of the chosen type.
     * @param <T> The type of the elements of the array. This must a type of `gc::object`.
     */
    template<class T>
    class array : public gc::object
    {
        private:
            gc::field<gc::dyn_array_impl<T>> _internal_array;
            END_GC_FIELDS;
        public:
            /*!
             * Creates an array of given size.
             * @param array_size Number of elements the array will be sized to. Arrays cannot be resized after their creation.
             */
            array(size_t array_size):
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
                gc::heap::heap_struct::get()->remove_from_initialization_list(_array_position); // The field expects the object to already have been removed from the initialization list
            }

            /*!
             * Accesses the elements of the array.
             * @param i Index of the array element to return.
             */
            gc::field<T>& operator[](int i)
            {
                gc_mark();
                return _internal_array[i];
            }
    };
}

#endif
