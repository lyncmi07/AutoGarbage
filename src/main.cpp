#include <iostream>

#include "gc.h"

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

int main()
{
    gc::init_gc(4096); // 4K
    std::cout << "Hello World!" << std::endl;

    /*for (unsigned int i = 0; i < 100; i++)
    {
        std::cout << "Creating " << i << std::endl;
        A* a = new A(i);
        std::cout << "Found size of " << a->size() << std::endl;


    }*/
    
    gc::static_ptr<B> b(new B());

    b->gc_mark();
    b->debug_gc();

    std::cout << "Inner B field A ";
    b->a().debug_object()->debug_gc();

    b->gc_mark();
    b->debug_gc();

    std::cout << "Inner B field A ";
    b->a().debug_object()->debug_gc();

    /*std::cout << "Inner B field A ";
    b.a().debug_object()->debug_gc();

    std::cout << "A number is :" << b.a()->i() << std::endl;

    b.debug_fields();

    std::cout << "Inner B field A ";
    b.a().debug_object()->debug_gc();
    
    b.a()->debug_fields();*/

    return 0;
}
