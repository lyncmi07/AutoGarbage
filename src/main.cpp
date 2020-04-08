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
    gc::init_gc(64);
    std::cout << "Hello World!" << std::endl;

    /*for (unsigned int i = 0; i < 100; i++)
    {
        std::cout << "Creating " << i << std::endl;
        A* a = new A(i);
        std::cout << "Found size of " << a->size() << std::endl;


    }*/
    
    B b;
    std::cout << "A number is :" << b.a()->i() << std::endl;

    b.debug_fields();
    b.a()->debug_fields();

    return 0;
}
