#ifndef GC_LIB_STATIC_PTR_H
#define GC_LIB_STATIC_PTR_H

#include "gc_heap.h"

namespace gc_test_access
{
    template<class T> T* debug_object(const gc::static_ptr<T>& static_ptr);
};

namespace gc
{
    template<class T> class field;
    class object;
    template<class T> class array;
    template<class T> class dyn_array_impl;

    template<class T> class static_ptr;

    template<class T> class static_ptr<gc::dyn_array_impl<T>>
    {
        private:
            gc::dyn_array_impl<T>* _object;
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
        public:
            static_ptr(gc::dyn_array_impl<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                gc::heap::heap_struct::get()->add_static(this);
            }

            // Copy constructors
            static_ptr(const gc::static_ptr<gc::dyn_array_impl<T>>& sptr2):
                _object(sptr2._object)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }
            gc::static_ptr<gc::dyn_array_impl<T>>& operator=(const gc::static_ptr<gc::dyn_array_impl<T>>& sptr2)
            {
                _object = sptr2._object;
                return *this;
            }

            operator gc::dyn_array_impl<T>*() const
            {
                return _object;
            }

            gc::static_ptr<gc::dyn_array_impl<T>>& operator=(gc::dyn_array_impl<T>* object)
            {
                _object = object;
                return *this;
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            template<class U> friend U* gc_test_access::debug_object(const gc::static_ptr<U>& static_ptr);
            friend class gc::object;
            friend class gc::heap::heap_struct;
    };

    template<class T> class static_ptr<gc::array<T>>
    {
        private:
            gc::array<T>* _object;
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
        public:
            static_ptr(gc::array<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                gc::heap::heap_struct::get()->add_static(this);
            }

            // Copy constructors
            static_ptr(const gc::static_ptr<gc::array<T>>& sptr2):
                _object(sptr2._object)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }
            gc::static_ptr<gc::array<T>>& operator=(const gc::static_ptr<gc::array<T>>& sptr2)
            {
                _object = sptr2._object;
                return *this;
            }

            operator gc::array<T>*() const
            {
                return _object;
            }

            gc::static_ptr<gc::array<T>>& operator=(gc::array<T>* object)
            {
                _object = object;
                return *this;
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            template<class U> friend U* gc_test_access::debug_object(const gc::static_ptr<U>& static_ptr);
            friend class gc::object;
            friend class gc::heap::heap_struct;
    };

    template<class T>
    class static_ptr
    {
        private:
            void ensure_gc_object(gc::object* object)
            {
            }

            T* _object;
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
        public:

            static_ptr():
                _object(nullptr)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }

            static_ptr(T* object):
                _object(object)
            {
                ensure_gc_object(object);
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                gc::heap::heap_struct::get()->add_static(this);
            }

            // Copy constructors
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

            operator T*() const
            {
                return _object;
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

            template<class U> friend U* gc_test_access::debug_object(const gc::static_ptr<U>& static_ptr);
            friend class gc::object;
            friend class gc::heap::heap_struct;
    };
}

#endif
