#ifndef GC_LIB_ARRAY_H
#define GC_LIB_ARRAY_H

#include "gc_cell.h"
#include "gc_object.h"
#include "gc_field.h"

#include <iostream>

namespace gc
{
    template<class T, int ARRAY_SIZE>
    class array : public gc::object
    {
        private:
            gc::field<T> _array[ARRAY_SIZE];
            END_GC_FIELDS;

        public:
            array()
            {
            }

            gc::field<T>& operator[](int i)
            {
                gc_mark();
                return _array[i];
            }
    };
}

#endif
