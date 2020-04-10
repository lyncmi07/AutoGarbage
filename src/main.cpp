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
    
    {
        gc::static_ptr<B> b(new B());
        gc::print_gc_debug();
    }
    gc::print_gc_debug();

    return 0;
}
