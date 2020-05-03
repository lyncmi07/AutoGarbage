#ifndef GC_LIB_OBJECT_H
#define GC_LIB_OBJECT_H

#include <iostream>

#include "gc_statics.h"
#include "gc_field.h"
#include "gc_cell.h"

namespace gc
{

    class object : public gc::heap::cell
    {
        private:
            char _mark;
            size_t _size;
            char _iteration;

            void gc_ecru();
            void gc_grey();
            void gc_black();
            void gc_grey_fields();

	    virtual bool* gc_fields_end();
        public:

            object();

            void* operator new(size_t size);

            size_t size();
            char current_mark();

	        void gc_mark();

	        void debug_fields();
	        void debug_gc();

            inline gc::object* fwd_object()
            {
                return (gc::object*) fwd_cell();
            }

            inline gc::object* back_object()
            {
                return (gc::object*) back_cell();
            }
    };

}

#endif
