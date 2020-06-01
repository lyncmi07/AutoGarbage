#ifndef GC_LIB_OBJECT_H
#define GC_LIB_OBJECT_H

#include <iostream>

#include "gc_statics.h"
#include "gc_cell.h"

/*!
 * This macro must be placed on inheritors of objects to signify the the marking system that there are no more fields left on the object.
 */
#define END_GC_FIELDS void* _end_gc_fields_magic_ptr = gc::heap::heap_struct::get()->end_gc_fields_magic_ptr(); bool* gc_fields_end() override { return ((bool*)&_end_gc_fields_magic_ptr); };

namespace gc
{
    namespace heap
    {
        class heap_struct;
    }
    template<class T> class array;
    template<class T> class dyn_array_impl;
    template<class T> class static_ptr;
    template<class T> class field;

    /*!
     * Users should override this class in order to make their object collectable by the AutoGarbage system.
     *
     * User defined objects are expected to have the pattern of garbage collectable fields at the top, followed by the `END_GC_FIELDS;` macro, followed 
     * by any remaining non garbage collectable members.
     *
     * Be careful when using raw/smart pointer members on `gc::object` classes as the destructor never gets called by the AutoGarbage system.
     *
     * Example class: 
     * ```
     * class Foo : public gc::object
     * {
     *     private:
     *         gc::field<Bar> _gcField;
     *         gc::field<Bar> _otherGcField;
     *         END_GC_FIELDS;
     *
     *         int _nonGcMember;
     *     public:
     *         Foo(Bar f1):
     *            _gcField(f1),
     *            _otherGcField(new Bar()),
     *            _nonGcMember(2)
     *         {
     *         }
     *
     *         void deReference()
     *         {
     *             _gcField = nullptr;
     *         }
     * }
     * ```
     */
    class object : public gc::heap::cell
    {
        private:
            char _mark;

            void gc_ecru();
            void gc_grey();
            void gc_black();
            void gc_grey_fields();

	          virtual bool* gc_fields_end();

            inline gc::object* fwd_object() const
            {
                return (gc::object*) fwd_treadmill();
            }

            inline gc::object* back_object() const
            {
                return (gc::object*) back_treadmill();
            }

            char current_mark() const;
	        virtual void gc_mark();
        public:

            object();

            /*!
             * Allocates a new object within the AutoGarbage library's heap.
             */
            void* operator new(size_t size);

            friend class gc::heap::heap_struct;
            template<class T> friend class gc::array;
            template<class T> friend class dyn_array_impl;
            template<class T> friend class gc::static_ptr;
            template<class T> friend class gc::field;
    };
}

#endif
