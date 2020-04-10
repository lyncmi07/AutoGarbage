#ifndef GC_LIB_STATIC_PTR_H
#define GC_LIB_STATIC_PTR_H

#include "gc_internal_functions.h"

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
            static_ptr<bool>* _back_static_object;
            static_ptr<bool>* _fwd_static_object;

            static_ptr(T* object):
                _object(object)
            {
                ensure_gc_object(object);
                gc::add_static(this);
            }

            T* operator->()
            {
                return _object;
            }

            void gc_mark()
            {
                _object->gc_mark();
            }
    };
}

#endif
