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
    gc::field<gc::array<A, 3>> _array;
    END_GC_FIELDS;

    public:
        C(): _array()
        {
        }
};

int main()
{
    // gc::heap::heap_struct::init_gc(2000);

    #if (PERFORMANCE_TIMERS)
        scoped_timer::init_timers();
    #endif

    gc::heap::heap_struct::init_gc(1000);

    int i = 0;
    /*while(true)
    {
        gc::static_ptr<B> b(new B());
        gc::heap::heap_struct::get()->print_gc_debug();
        // std::cout << "loop:" << i++ << " ";
        // gc::heap::heap_struct::get()->print_gc_info();
    }*/

    // gc::static_ptr<B> b(new B());
    // b->_a = new A(2);

    gc::static_ptr<C> c(new C());
    // gc::static_ptr<C> c2(new C());
    // gc::static_ptr<C> c3(new C());

    c->_array[0] = new A(15);

    for (int i = 0; i < 3; i++)
    {
        c->_array[i] = new A(i+10);
    }

    for (int i = 0; i < 3; i++)
    {
        std::cout << c->_array[i]->i() << std::endl;
    }

    #if (PERFORMANCE_TIMERS)
        scoped_timer::print_info();
    #endif

    return 0;
}

void debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
}
