#ifndef GC_LIB_FIELD_H
#define GC_LIB_FIELD_H

#include "gc_heap.h"

namespace gc_test_access
{
    template<class U> U* debug_object(const gc::field<U>& field);
};

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

            // Copy constructors
            field(const field<T> &f2):
                _object(f2._object)
            {
            }
            field<gc::dyn_array_impl<T>>& operator=(const field<gc::dyn_array_impl<T>>& f2)
            {
                _object = f2._object;
                return *this;
            }

            field<gc::dyn_array_impl<T>>& operator=(gc::dyn_array_impl<T>* object)
            {
                _object = object;

                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                return *this;
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

	          void gc_mark()
	          {
		            if (_object != nullptr) _object->gc_mark();
	          }

            template<class U> friend U* gc_test_access::debug_object(const gc::field<U>& field);
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

            // Copy constructors
            field(const field<T> &f2):
                _object(f2._object)
            {
            }
            field<gc::array<T>>& operator=(const field<gc::array<T>>& f2)
            {
                _object = f2._object;
                return *this;
            }

            field<gc::array<T>>& operator=(gc::array<T>* object)
            {
                _object = object;

                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                return *this;
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

	          void gc_mark()
	          {
		            if (_object != nullptr) _object->gc_mark();
	          }

            template<class U> friend U* gc_test_access::debug_object(const gc::field<U>& field);
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

            // Copy constructors
            field(const field<T> &f2):
                _object(f2._object)
            {
            }
            field<T>& operator=(const field<T>& f2)
            {
                _object = f2._object;
                return *this;
            }

            field<T>& operator=(T* o2)
            {
                _object = o2;
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);

                return *this;
            }

            operator T*() const
            {
                return _object;
            }

	          T* operator->()
	          {
		            if (_object != nullptr) _object->gc_mark();
                return _object;
	          }

            

	          void gc_mark()
	          {
		            if (_object != nullptr) _object->gc_mark();
	          }

            template<class U> friend U* gc_test_access::debug_object(const gc::field<U>& field);
    };
}
#endif
