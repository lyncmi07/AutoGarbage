#include <iostream>

#include "gc.h"

class A : public gc::object
{
    private:
        unsigned int _i;

    public:
        A(unsigned int i):
        _i(i)
        {
        }
};

class B : public gc::object
{
    private:
        gc::field<A> _a;
    public:
        B(): _a(new A(1))
        {
        }
};

int main()
{
    gc::init_gc(64);
    std::cout << "Hello World!" << std::endl;

    for (unsigned int i = 0; i < 100; i++)
    {
        std::cout << "Creating " << i << std::endl;
        A* a = new A(i);
        std::cout << "Found size of " << a->size() << std::endl;
    }

    return 0;
}
