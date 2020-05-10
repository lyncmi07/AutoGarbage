#ifndef GC_LIB_FIELD_H
#define GC_LIB_FIELD_H

#include "gc_heap.h"

namespace gc
{
    class object;
    template<class T, int ARRAY_SIZE> class array;

    template<class T> class field;

    template<class S, int ARRAY_SIZE> class field<gc::array<S, ARRAY_SIZE>>
    {
        private:
            gc::array<S, ARRAY_SIZE>* _object;
        public:
            field():
                _object(new gc::array<S, ARRAY_SIZE>())
            {
            }


            gc::field<S>& operator[](int i)
            {
                return (*_object)[i];
            }
    };

    template<class T> class field
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

            field<T>& operator=(T* o2)
            {
                _object = o2;
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);

                return *this;
            }

            field<T>& operator=(const field<T>& f2)
            {
                _object = f2._object;
                return *this;
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

            inline bool holds_valid_object()
            {
                return _object != nullptr;
            }
    };
}
#endif
