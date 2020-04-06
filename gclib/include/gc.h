#ifndef GC_LIB_H
#define GC_LIB_H

#include <unordered_set>

namespace gc
{

    class object;
    template<class T> class field;

    size_t _heap_size = 0;

    char* _heap_space = nullptr;
    size_t next_ptr_offset = 0;

    std::unordered_set<gc::object*> _white_objects;
    std::unordered_set<gc::object*> _black_objects;

    unsigned int _current_grey_objects = 0;

    void init_gc(size_t heap_size)
    {
        _heap_size = heap_size;
        _heap_space = new char[_heap_size];
    }

    void* malloc(size_t size)
    {
        if (next_ptr_offset + size >= _heap_size) throw std::bad_alloc();

	char* pc = &(_heap_space[next_ptr_offset]);
        void* p = (void*) pc;
        next_ptr_offset += size;

        return p;
    }

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

            }

        public:

            object():
                _mark('W')
            {
            }

            void* operator new(size_t size)
            {
                std::cout << "The size is " << size << std::endl;

                void* p = gc::malloc(size);
                ((object*) p)->_size = size;

                return p;
            }

            size_t size()
            {
                return _size;
            }
    };

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
    };

}

#endif
