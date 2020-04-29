#include <iostream>
#include <chrono>
#include <string>

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
};

int main()
{
    std::cout << "Running performance test" << std::endl;

    gc::heap::heap_struct::init_gc(4000);

    {
        Timer allAllocationsTimer("1,000,000,000 Allocations");
        for(unsigned long i = 0; i < 1000000000; i++)
        {
            gc::static_ptr<RandomSizeObject> rso(new RandomSizeObject());
            gc::heap::heap_struct::get()->print_gc_debug();
        }
    }
}
