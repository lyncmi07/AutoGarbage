#include <iostream>

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
    private:
        gc::field<A> _a;
	END_GC_FIELDS;

    public:

        B(): _a(new A(1))
        {
        }

	gc::field<A>& a()
	{
            return _a;
	}
};

/*int main()
{
    // gc::init_gc(4096); // 4K
    gc::heap::heap_struct::init_gc(4096);
    std::cout << "Hello World!" << std::endl;
    
    {
        gc::static_ptr<B> b(new B());
        gc::heap::heap_struct::get()->print_gc_debug();

        b->debug_gc();
        b->a().debug_object()->debug_gc();
        b->a()->debug_gc();
        b->debug_gc();
    }
    gc::heap::heap_struct::get()->print_gc_debug();

    int a;
    // std::cin >> a;

    gc::heap::heap_struct::get()->flip();
    gc::heap::heap_struct::get()->print_gc_debug();

    gc::heap::heap_struct::get()->flip();
    gc::heap::heap_struct::get()->print_gc_debug();

    {
        gc::static_ptr<B> b(new B());
        gc::heap::heap_struct::get()->print_gc_debug();
        b->a()->debug_gc();
        gc::heap::heap_struct::get()->print_gc_debug();
    }
    gc::heap::heap_struct::get()->print_gc_debug();

    return 0;
}*/

int main()
{
}

void debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
}
