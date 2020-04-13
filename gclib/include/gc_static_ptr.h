#ifndef GC_LIB_STATIC_PTR_H
#define GC_LIB_STATIC_PTR_H

#include "gc_heap.h"

namespace gc
{
    class object;

    template<class T>
    class static_ptr
    {
        private:
            void ensure_gc_object(gc::object* object)
            {
            }

            T* _object;
        public:
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;

            static_ptr(T* object):
                _object(object)
            {
                ensure_gc_object(object);
                gc::heap::heap_struct::get()->add_static(this);
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            T* operator->()
            {
                _object->gc_mark();
                return _object;
            }

            void gc_mark()
            {
                _object->gc_mark();
            }
    };
}

#endif
