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

    /*!
     * static_ptr is used to keep a scoped reference to a garbage collectable object.
     * Once the static_ptr object goes out of scope, the reference to the garbage collectable object is removed and
     * can be collected as long as it is not referenced anywhere else.
     *
     * @param <T> The type of `gc::object` which is going to be used as elements to the referenced array.
     */
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

            /*!
             * New static_ptr with null reference.
             */
            static_ptr():
                _object(nullptr)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }
                
            /*!
             * Assigns the static_ptr to the newly given array object.
             * @param object New array object to reference
             */
            static_ptr(gc::array<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                gc::heap::heap_struct::get()->add_static(this);
            }

            /*!
             * Copy constructor.
             */
            static_ptr(const gc::static_ptr<gc::array<T>>& sptr2):
                _object(sptr2._object)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }
            
            /*!
             * Copy assignment. Object is not recreated.
             */
            gc::static_ptr<gc::array<T>>& operator=(const gc::static_ptr<gc::array<T>>& sptr2)
            {
                _object = sptr2._object;
                return *this;
            }

            /*!
             * Assigns the static_ptr to the newly given array object.
             * @param object New array object to reference.
             */
            gc::static_ptr<gc::array<T>>& operator=(gc::array<T>* object)
            {
                _object = object;
                return *this;
            }

            /*!
             * static_ptr will implicitly cast to the referenced array.
             */
            operator gc::array<T>*() const
            {
                return _object;
            }

            /*!
             * Accesses the elements of the referenced array.
             * @param i Index of the array to return.
             */
            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            /*!
             * Once unscoped, the held object is now free to be collected (if it is not being referenced somewhere else).
             */
            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            template<class U> friend U* gc_test_access::debug_object(const gc::static_ptr<U>& static_ptr);
            friend class gc::object;
            friend class gc::heap::heap_struct;
    };

    /*!
     * static_ptr is used to keep a scoped reference to a garbage collectable object.
     * Once the static_ptr object goes out of scope, the reference to the garbage collectable object is removed and
     * can be collected as long as it is not referenced anywhere else.
     *
     * @param <T> The type of `gc::object` which is going to be referenced.
     *
     */
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

            /*!
             * New static_ptr with null reference.
             */
            static_ptr():
                _object(nullptr)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }

            /*!
             * Assigns the static_ptr to the newly given object.
             * @param object New object to reference
             */
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

            /*!
             * Copy constructor.
             */
            static_ptr(const gc::static_ptr<T>& sptr2):
                _object(sptr2._object)
            {
                gc::heap::heap_struct::get()->add_static(this);
            }

            /*!
             * Copy assignment. Object is not recreated.
             */
            gc::static_ptr<T>& operator=(const gc::static_ptr<T>& sptr2)
            {
                _object = sptr2._object;
                return *this;
            }

            /*!
             * Assigns the static_ptr to the newly given object.
             * @param object New object to reference.
             */
            gc::static_ptr<T>& operator=(T* object)
            {
                _object = object;
                return *this;
            }

            /*!
             * static_ptr will implicitly cast to the referenced object.
             */
            operator T*() const
            {
                return _object;
            }

            /*!
             * Once unscoped, the held object is now free to be collected (if it is not being referenced somewhere else).
             */
            ~static_ptr()
            {
                gc::heap::heap_struct::get()->remove_static(this);
            }

            /*!
             * Accesses members of the referenced object.
             */
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
