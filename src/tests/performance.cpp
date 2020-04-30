#include <iostream>
#include <chrono>
#include <string>

#include <cstdlib>
#include <time.h>

#include "gc.h"

class Timer
{
private:
    const std::chrono::time_point<std::chrono::steady_clock> _startTime;
    const std::string _name;

public:
    Timer(const std::string& name): _startTime(std::chrono::steady_clock::now()), _name(name)
    {
    }

    ~Timer()
    {
        std::chrono::time_point<std::chrono::steady_clock> _endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime - _startTime);

        std::cout << "Timer '" << _name << "' ended: " << duration.count() << std::endl;
    }
};

class RandomSizeObject : public gc::object
{
    END_GC_FIELDS;
    int i;

public:
    RandomSizeObject(): i(0)
    {
    }

    void* operator new(size_t size)
    {
        return gc::object::operator new(size + (rand() & 100));
    }
};

int main()
{
    //initialise random
    srand(time(NULL));

    std::cout << "Running performance test" << std::endl;

    gc::heap::heap_struct::init_gc(4000);

    {
        Timer allAllocationsTimer("1,000,000,000 Allocations");
        for(unsigned long i = 0; i < 1000000000; i++)
        {
            {
                Timer singleAllocationTimer("Single allocation");
                gc::static_ptr<RandomSizeObject> rso(new RandomSizeObject());
            }
            gc::heap::heap_struct::get()->print_gc_info();
        }
    }
}

void debug()
{
    gc::heap::heap_struct::get()->print_gc_debug();
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
