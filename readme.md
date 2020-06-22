# C++ AutoGarbage
AutoGarbage is an automatic garbage collector for the C++ Programming Language, implementing the treadmill no-motion mark and sweep technique.

## Usage

To start using the AutoGarbage system include `gclib/include/gc.h` within your relevant source files and remember to link `gclib/lib/libgc.a` library at link time.

Classes that take advantage of the collector can be written by inheriting from the `gc::object` class:

``` C++
class A : gc::object
{
    //These are garbage collectable objects.
    gc::field<B> _b;
    gc::field<C> _c;

    //This notifies to the garbage collector that there are no more fields in the object
    END_GC_FIELDS;

    //All non-gc members should appear below the garbage collectable fields
    int _n;
    int _m;

    class A(B* b):
        _b(b),
        _c(new C()),
        _n(10),
        _m(20)
    {
    }
}
```

To start the AutoGarbage system use:
``` C++
//4096 byte heap with an maximum allocator retry of 25
gc::init_gc(4096, 25)
```

AutoGarbage also supports reference arrays to garbage collectable objects:
``` C++
gc::static_ptr<gc::array<B>> b_array = new gc::array<B>(10); //Creates a static pointer (which is removed when unscoped) to an array
```

## Building
To build, call `make`, this will produce the `gclib/lib/libgc.a` library file. To use the library this must be included at the linker phase
as well as using all the header files found in `gclib/include`

## Find out more
Find more detail on this project by reading the [[report]] on how it works and using the [[documentation]]
