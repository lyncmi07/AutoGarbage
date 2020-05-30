#ifndef GC_LIB_FIELD_H
#define GC_LIB_FIELD_H

#include "gc_heap.h"

namespace gc
{

    class object;
    template<class T> class array;
    template<class T> class dyn_array_impl;

    template<class T> class field;

    template<class T> class field<gc::dyn_array_impl<T>>
    {
        private:
            gc::dyn_array_impl<T>* _object;
        public:
            field():
                _object(nullptr)
            {
            }

            field(gc::dyn_array_impl<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            gc::dyn_array_impl<T>* debug_object()
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

    template<class T> class field<gc::array<T>>
    {
        private:
            gc::array<T>* _object;
        public:
            field():
                _object(nullptr)
            {
            }

            field(gc::array<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            gc::array<T>* debug_object()
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
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
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

            field<T>& operator=(static_ptr<T>& sptr)
            {
                _object = sptr.debug_object();
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
