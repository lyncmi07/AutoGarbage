#include <iostream>
#include <chrono>
#include <string>

#include <cstdlib>
#include <time.h>

#include "gc_scoped_timer.h"
#include "gc.h"

class Timer
{
private:
    const std::chrono::time_point<std::chrono::steady_clock> _startTime;
    const std::string _name;
    const double _divisor;

public:
    Timer(const std::string& name, const double divisor):
        _startTime(std::chrono::steady_clock::now()),
        _name(name),
        _divisor(divisor)
    {
    }

    ~Timer()
    {
        std::chrono::time_point<std::chrono::steady_clock> _endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(_endTime - _startTime);

        std::cout << "Timer '" << _name << "' ended: "
            << ((duration.count() * 1.0) / _divisor)
            << " * " << _divisor;
            // << std::endl;
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
    #if (PERFORMANCE_TIMERS)
        scoped_timer::init_timers();
    #endif

    //initialise random
    // srand(time(NULL));
    srand(23102);

    std::cout << "Running performance test" << std::endl;

    gc::init(10000000, 25);

    {
        Timer allAllocationsTimer("1,000,000 Allocations", 1000000.0);
        for(unsigned long i = 0; i < 1000000; i++)
        {
            {
                gc::static_ptr<RandomSizeObject> rso(new RandomSizeObject());
            }
            // gc::heap::heap_struct::get()->print_gc_info();
            // gc::heap::heap_struct::get()->print_gc_debug();
        }

    }

    std::cout << std::endl;
    gc::heap::heap_struct::get()->print_gc_info();

    #if (PERFORMANCE_TIMERS)
        scoped_timer::print_info();
    #endif
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
