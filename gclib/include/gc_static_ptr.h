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

            static_ptr():
                _object(nullptr)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }

            static_ptr(T* object):
                _object(object)
            {
                ensure_gc_object(object);
                if (object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(object);
                gc::heap::heap_struct::get()->add_static(this);
            }

            static_ptr(const gc::static_ptr<T>& sptr2):
                _object(sptr2._object)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }

            gc::static_ptr<T>& operator=(const gc::static_ptr<T>& sptr2)
            {
                _object = sptr2._object;
                return *this;
            }

            gc::static_ptr<T>& operator=(T* object)
            {
                _object = object;
                return *this;
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            T* operator->()
            {
                if (_object != nullptr) _object->gc_mark();
                return _object;
            }

            T* debug_object()
            {
                return _object;
            }

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
    };
}

#endif
