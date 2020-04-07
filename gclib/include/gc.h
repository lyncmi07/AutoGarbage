#ifndef GC_LIB_H
#define GC_LIB_H

#include <unordered_set>

#include "object.h"
#include "gc_statics.h"

namespace gc
{

    class object;

    void init_gc(size_t heap_size)
    {
        _heap_size = heap_size;
        _heap_space = new char[_heap_size];
    }

    void* malloc(size_t size)
    {
        if (next_ptr_offset + size >= _heap_size) throw std::bad_alloc();

	char* pc = &(_heap_space[next_ptr_offset]);
        void* p = (void*) pc;
        next_ptr_offset += size;

        return p;
    }


    template<class T>
    class field
    {
        private:
            void ensure_gc_object(gc::object* object)
            {
            }

            T* _object;
        public:
            field(T* object):
            _object(object)
            {
                ensure_gc_object(object);
            }
    };

}

#endif
