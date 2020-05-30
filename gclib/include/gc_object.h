#ifndef GC_LIB_OBJECT_H
#define GC_LIB_OBJECT_H

#include <iostream>

#include "gc_statics.h"
#include "gc_cell.h"

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

            void* operator new(size_t size);

            friend class gc::heap::heap_struct;
            template<class T> friend class gc::array;
            template<class T> friend class dyn_array_impl;
            template<class T> friend class gc::static_ptr;
            template<class T> friend class gc::field;
    };
}

#endif
