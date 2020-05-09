#ifndef GC_LIB_FIELD_H
#define GC_LIB_FIELD_H

#include "gc_heap.h"

namespace gc
{
    class object;

    template<class T>
    class field
    {
        private:
            void ensure_gc_object(gc::object* object)
            {
            }

            T* _object;
        public:
            field():
                _object(nullptr)
            {
            }

            field(T* object):
                _object(object)
            {
                ensure_gc_object(object);
                if (object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(object);
            }

            field(const field<T> &f2):
                _object(f2._object)
            {
            }

            field<T>& operator=(const field<T> &f2)
            {
                _object = f2._object;
            }

            field<T>& operator=(const T &o2)
            {
                _object = o2;
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
            }

	        T* operator->()
	        {
		    _object->gc_mark();
                    return _object;
	        }

	        T* debug_object()
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
