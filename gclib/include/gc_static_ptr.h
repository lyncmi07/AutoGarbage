#ifndef GC_LIB_STATIC_PTR_H
#define GC_LIB_STATIC_PTR_H

#include "gc_heap.h"

namespace gc
{
    template<class T> class field;
    class object;
    template<class T, int ARRAY_SIZE> class array;
    template<class T> class dyn_array;
    template<class T> class dyn_array_impl;

    template<class T> class static_ptr;

    template<class T> class static_ptr<gc::dyn_array_impl<T>>
    {
        private:
            gc::dyn_array_impl<T>* _object;
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;
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

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            gc::dyn_array_impl<T>* debug_object()
            {
                return _object;
            }

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
    };

    template<class T> class static_ptr<gc::dyn_array<T>>
    {
        private:
            gc::dyn_array<T>* _object;
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;
        public:
            static_ptr(gc::dyn_array<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                gc::heap::heap_struct::get()->add_static(this);
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            gc::dyn_array<T>* debug_object()
            {
                return _object;
            }

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
    };

    template<class S, int ARRAY_SIZE> class static_ptr<gc::array<S, ARRAY_SIZE>>
    {
        private:
            gc::array<S, ARRAY_SIZE>* _object;
            static_ptr<gc::object>* _back_static_object;
            static_ptr<gc::object>* _fwd_static_object;
        public:
            static_ptr():
                _object(new gc::array<S, ARRAY_SIZE>())
            {
            }

            gc::field<S>& operator[](int i)
            {
                return (*_object)[i];
            }

            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            gc::array<S, ARRAY_SIZE>* debug_object()
            {
                return _object;
            }

            void gc_mark()
            {
                if (_object != nullptr) _object->gc_mark();
            }
    };

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
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
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

            gc::static_ptr<T>& operator=(const gc::field<T>& field)
            {
                _object = field._object;
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
