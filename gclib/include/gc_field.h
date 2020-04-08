#ifndef GC_LIB_FIELD_H
#define GC_LIB_FIELD_H

namespace gc
{
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
                return _object;
	    }
    };
}
#endif
