#ifndef GC_LIB_OBJECT_H
#define GC_LIB_OBJECT_H

#include <iostream>

#include "gc_statics.h"
#include "gc_functions.h"
#include "gc_field.h"

namespace gc
{

    class object
    {
        private:
            char _mark;
            size_t _size;

            void gc_white()
            {
                _mark = 'W';
            }

            void gc_grey()
            {
                _mark = 'G';
                gc::_white_objects.erase(this);
                _current_grey_objects++;
            }

            void gc_black()
            {
                _mark = 'B';
                _current_grey_objects--;
                gc_grey_fields();
                gc::_black_objects.insert(this);
            }

            void gc_grey_fields()
            {
		gc::field<gc::object>* field_ptr = (gc::field<gc::object>*) (((unsigned long) this) + sizeof(void*));
		bool* fields_end_ptr = gc_fields_end();

		while ((bool*)field_ptr < fields_end_ptr)
		{
		    (*field_ptr)->gc_mark();
		}
            }

	    virtual bool* gc_fields_end() { return nullptr; }
        public:

            object():
                _mark('W')
            {
            }

            void* operator new(size_t size)
            {
                void* p = gc::malloc(size);
                ((object*) p)->_size = size;

                return p;
            }

            size_t size()
            {
                return _size;
            }

	    void gc_mark()
	    {
	        switch (_mark) //seg fault
		{
	            case 'B':
	                return;
		    case 'G':
			gc_black();
			break;
		    case 'W':
			gc_grey();
			break;
		}
	    }

	    void debug_fields()
	    {
                std::cout << "The object starts at: " << this << std::endl;
		std::cout << "The fields end at: " << ((unsigned long)gc_fields_end()) - ((unsigned long)this) << std::endl;
	    }

	    void debug_gc()
	    {
                std::cout << "Marked: " << _mark << std::endl;
	    }
    };

}

#endif
