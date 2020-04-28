#include <iostream>

#include "gc.h"
#include "gc_heap.h"

class SClass
{
    public:
    char c;
};
class TClass : public SClass
{
    public:
    char a;
    char b;
};

class NClass
{
    public:
    char d;
};

class UClass : public NClass
{
    public:
    char _c;

    virtual char c()
    {
        return _c;
    }
};
class VClass : public UClass
{
    public:
    char a;
    char b;
    
    char c() override
    {
        return a;
    }
};

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
    TClass c;
    VClass v;

    TClass* cp = new TClass();
    VClass* vp = new VClass();

    std::cout << sizeof(c) << std::endl;
    std::cout << sizeof(v) << std::endl;
    std::cout << sizeof(cp) << std::endl;
    std::cout << sizeof(vp) << std::endl;

    gc::heap::heap_struct::init_gc(200000);
    // gc::heap::heap_struct::init_gc(1000);

    int i = 0;
    while(true)
    {
        gc::static_ptr<B> b(new B());
        // gc::heap::heap_struct::get()->print_gc_debug();
        std::cout << "loop:" << i++ << " ";
        gc::heap::heap_struct::get()->print_gc_info();
    }


    return 0;
}

void debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
}
