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

	          void gc_mark()
	          {
		            if (_object != nullptr) _object->gc_mark();
	          }
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

            operator gc::dyn_array_impl<T>*() const
            {
                return _object;
            }

            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            template<class U> friend U* gc_test_access::debug_object(const gc::field<U>& field);
            friend class gc::object;
    };

    /*!
     * Fields are used as garbage collectable members on inheritors of `gc::object`.
     * Fields must appear as the first members of the object in order for the marker to find them.
     * After all fields have been defined they must be suffixed with the `END_GC_FIELDS;` macro;
     *
     * @param <T> The type of `gc::object` which is going to be used as elements to the referenced array.
     */
    template<class T> class field<gc::array<T>>
    {
        private:
            gc::array<T>* _object;

	          void gc_mark()
	          {
		            if (_object != nullptr) _object->gc_mark();
	          }

        public:
            /*!
             * New field with null reference.
             */
            field():
                _object(nullptr)
            {
            }

            /*!
             * New field pointing to given array object.
             *
             * @param array_object Array object to reference.
             */
            field(gc::array<T>* array_object):
                _object(array_object)
            {
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
            }

            /*!
             * Copy constructor.
             */
            field(const field<T> &f2):
                _object(f2._object)
            {
            }

            /*!
             * Copy assignment. Object is not recreated.
             */
            field<gc::array<T>>& operator=(const field<gc::array<T>>& f2)
            {
                _object = f2._object;
                return *this;
            }

            /*!
             * Assigns the field to the newly given array object.
             * @param object New array object to reference.
             */
            field<gc::array<T>>& operator=(gc::array<T>* object)
            {
                _object = object;

                if (_object != nullptr && _object->current_mark() == 'I')
                {
                    gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
                }
                return *this;
            }

            /*!
             * field will implicitly cast to the referenced array.
             */
            operator gc::array<T>*() const
            {
                return _object;
            }

            /*!
             * Accesses the elements of the referenced array.
             * @param i Index of the array elemtn to return.
             */
            gc::field<T>& operator[](int i)
            {
                return (*_object)[i];
            }

            template<class U> friend U* gc_test_access::debug_object(const gc::field<U>& field);
            friend class gc::object;
    };

    /*!
     * Fields are used as garbage collectable members on inheritors of `gc::object`.
     * Fields must appear as the first members of the object in order for the marker to find them.
     * After all fields have been defined they must be suffixed with the `END_GC_FIELDS;` macro;
     *
     * @param <T> The type of `gc::object` which is going to be referenced.
     *
     */
    template<class T> class field
    {
        private:
            void ensure_gc_object(gc::object* object)
            {
            }

            T* _object;

	          void gc_mark()
	          {
		            if (_object != nullptr) _object->gc_mark();
	          }
        public:
            /*!
             * New field with null reference.
             */
            field():
                _object(nullptr)
            {
            }

            /*!
             * New field pointing to given object.
             *
             * @param object Object to reference.
             */
            field(T* object):
                _object(object)
            {
                ensure_gc_object(object);
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);
            }

            /*!
             * Copy constructor.
             */
            field(const field<T> &f2):
                _object(f2._object)
            {
            }

            /*!
             * Copy assignment.
             */
            field<T>& operator=(const field<T>& f2)
            {
                _object = f2._object;
                return *this;
            }

            /*!
             * Assigns the field to the newly given object.
             * @param object New object to reference.
             */
            field<T>& operator=(T* o2)
            {
                _object = o2;
                if (_object != nullptr && _object->current_mark() == 'I') gc::heap::heap_struct::get()->remove_from_initialization_list(_object);

                return *this;
            }

            /*!
             * Field will implicitly cast to the referenced array.
             */
            operator T*() const
            {
                return _object;
            }

            /*!
             * Accesses members of the referenced object.
             */
	          T* operator->()
	          {
		            if (_object != nullptr) _object->gc_mark();
                return _object;
	          }
            

            template<class U> friend U* gc_test_access::debug_object(const gc::field<U>& field);
            friend class gc::object;
    };
}
#endif
