#include <iostream>

#if defined(PERFORMANCE_TIMERS)
#include "gc_scoped_timer.h"
#endif

#include "gc.h"
#include "gc_heap.h"

class A : public gc::object
{
    private:
	END_GC_FIELDS;

        unsigned int _i;

    public:
        A(unsigned int i):
        _i(i)
        {
        }

	unsigned int i()
	{
            return _i;
	}
};

class B : public gc::object
{
    public:
        gc::field<A> _a;
        // gc::field<gc::array<A, 3>> _array;
	    END_GC_FIELDS;

    public:

        B(): _a(new A(1))
        {
            /*for (unsigned int i = 0; i < 3; i++)
            {
                _array[i] = new A(i);
            }*/
        }

	    gc::field<A>& a()
	    {
            return _a;
	    }
};

class C : public gc::object
{
    public:
    gc::field<gc::dyn_array<A>> _array;
    END_GC_FIELDS;

    public:
        C(size_t size): _array(new gc::dyn_array<A>(size))
        {
        }
};

int main()
{
    #if (PERFORMANCE_TIMERS)
        scoped_timer::init_timers();
    #endif

    gc::init(10000, 25);

    gc::static_ptr<C> c(new C(10));

    c->_array[0] = new A(10);
    std::cout << c->_array[0]->i() << std::endl;
    c->_array[0] = new A(9);
    c->_array[10] = new A(9);

    std::cout << c->_array[1]->i() << std::endl;
    std::cout << c->_array[0]->i() << std::endl;

    gc::debug();

    #if (PERFORMANCE_TIMERS)
        scoped_timer::print_info();
    #endif

    return 0;
}

void debug()
{
    gc::debug();
}

void dump_memory(void* position, int start, int end)
{
    
    char* char_position = (char*) position;

    unsigned int lineSeparation = 8;
    for (unsigned int i = 0; i < (end - start); i++)
    {
        if (i % lineSeparation == 0) printf("\n");
        printf("%02hhx ", (int)char_position[start + i]);
    }

    std::cout << std::endl;
}
