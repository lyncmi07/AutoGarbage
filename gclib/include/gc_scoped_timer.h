#ifndef GC_LIB_SCOPED_TIMER
#define GC_LIB_SCOPED_TIMER


#include <iostream>
#include <chrono>
#include <string>
#include <cstring>

/*
 * Example Usage:
 *
 * scoped_timer::init_timers();
 * {
 *     scoped_timer t(timer_group::A)
 *     std::cout << "Hello World!" << std::endl;
 * }
 *
 * scoped_timer::print_info();
 *
 */

#define TIMER_GROUP(group_names...) enum timer_group { group_names, size }; \
    static std::string merged_timer_group_names = #group_names;


// Replace letters with chosen timer names
TIMER_GROUP(
        TIMER_MALLOC,
        TIMER_ATTEMPT_ALLOCATE,
        TIMER_COLLECT_GARBAGE,
        TIMER_GREY_MARKING,
        TIMER_ATTEMPT_ALLOCATE_LOOP,
        TIMER_CELL_RESIZE
)

class scoped_timer
{
    private:

    inline static const double TIMER_OVERHEAD = 17.7669; //Expected overhead of 17.7669 nanoseconds
    inline static std::string* timer_group_names;
    inline static long* group_total_time;
    inline static long* group_call_counter;

    const timer_group _group;
    const std::chrono::time_point<std::chrono::steady_clock> _start_time;

    static void populate_timer_group_names()
    {
        scoped_timer::timer_group_names = new std::string[timer_group::size];

        std::string current_string = merged_timer_group_names;
        unsigned int current_pos = 0;
        for (unsigned int i = 0; i < timer_group::size; i++)
        {
            std::string::size_type next_pos = current_string.find(",");
            timer_group_names[i] = current_string.substr(current_pos, (next_pos - current_pos));

            current_string = current_string.substr(next_pos+1);
        }
    }

    public:

    //Reset timers
    static void reset_timers()
    {
        delete [] scoped_timer::timer_group_names;
        scoped_timer::timer_group_names = nullptr;

        delete [] scoped_timer::group_total_time;
        scoped_timer::group_total_time = nullptr;

        delete [] scoped_timer::group_call_counter;
        scoped_timer::group_call_counter = nullptr;

        init_timers();
    }

    //Call to initiate timer variables
    static void init_timers()
    {
        populate_timer_group_names();

        scoped_timer::group_total_time = new long[timer_group::size];
        memset(scoped_timer::group_total_time, 0, sizeof(long) * timer_group::size);

        scoped_timer::group_call_counter = new long[timer_group::size];
        memset(scoped_timer::group_call_counter, 0, sizeof(long) * timer_group::size);
    }

    //Print timer total calls and average call times
    static void print_info()
    {
        std::cout << "--Timer Stats--" << std::endl;
        for (unsigned int i = 0; i < timer_group::size; i++)
        {
            double average_time = (scoped_timer::group_total_time[i] * 1.0) / scoped_timer::group_call_counter[i];
            average_time -= TIMER_OVERHEAD;
            std::cout << scoped_timer::timer_group_names[i]
                << ": avg:" << average_time
                << " total_time:" << scoped_timer::group_total_time[i] 
                << " total_calls:" << scoped_timer::group_call_counter[i] << std::endl;
        }
    }

    //Create a timer at the start of the scope you want to time and provide the group name of the timer
    scoped_timer(timer_group group):
        _group(group),
        _start_time(std::chrono::steady_clock::now())
    {
    }

    ~scoped_timer()
    {
        std::chrono::time_point<std::chrono::steady_clock> _end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(_end_time - _start_time);

        scoped_timer::group_call_counter[_group]++;
        scoped_timer::group_total_time[_group] += duration.count();
    }
};

#endif
