#ifndef GC_LIB_FIELD_H
#define GC_LIB_FIELD_H

namespace gc
{
    class object;

    template<class T>
    class field
    {
        private:
            void ensure_gc_object(gc::object* object)
            {
            }

            T* _object;
        public:
            field(T* object):
                _object(object)
            {
                ensure_gc_object(object);
            }

	    T* operator->()
	    {
		_object->gc_mark();
                return _object;
	    }

	    T* debug_object()
	    {
	        return _object;
	    }

	    void gc_mark()
	    {
		_object->gc_mark();
	    }
    };
}
#endif
