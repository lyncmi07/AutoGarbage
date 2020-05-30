#ifndef GC_TESTS_TEST_H
#define GC_TESTS_TEST_H

#include <iostream>

#include "gc_cell.h"
#include "gc_heap.h"
#include "gc_object.h"
#include "gc_test_access.h"

#define TEST(func_name) int func_name()

#define TEST_FAIL return 1
#define TEST_SUCCESS return 0

#define RUN_TEST(func_name) if(func_name() == 1) { std::cout << #func_name << " failed." << std::endl; return 1; } else { std::cout << #func_name << " passed." << std::endl; }
#define RUN_TEST_SUITE(func_name) if(func_name::run_tests() == 1) { std::cout << #func_name << " failed." << std::endl; return 1; } else { std::cout << #func_name << " suite passed." << std::endl; }

#define ASSERT_BOTTOM(cell) { auto c = cell; if(assert_bottom(c) == 1) { std::cout << #cell << ":" << c << " is not in bottom list" << std::endl; return 1; } }
#define ASSERT_TOP(cell) { auto c = cell; if(assert_top(c) == 1) { std::cout << #cell << ":" << c << " is not in top list" << std::endl; return 1; } }
#define ASSERT_SCAN(cell) { auto c = cell; if(assert_scan(c) == 1) { std::cout << #cell << ":" << c << " is not in scan list" << std::endl; return 1; } }
#define ASSERT_FREE(cell) { auto c = cell; if(assert_free(c) == 1) { std::cout << #cell << ":" << c << " is not in free list" << std::endl; return 1; } }

#define ASSERT_EQUALS(a, b) { if (a != b) { std::cout << a << " is not equal to " << b << std::endl; return 1; } }

int assert_in_list(gc::heap::cell* first_list_cell, gc::heap::cell* list_end_cell, gc::heap::cell* cell)
{
    gc::heap::cell* current_cell = first_list_cell;

    while (current_cell != list_end_cell)
    {
        if (current_cell == cell) return 0;
        current_cell = current_cell->fwd_treadmill();
    }

    return 1;
}

int assert_bottom(gc::heap::cell* cell)
{
    return assert_in_list(
            gc_test_access::bottom(),
            gc_test_access::top()->back_treadmill(),
            cell);
}

int assert_top(gc::heap::cell* cell)
{
    return assert_in_list(
            gc_test_access::top(),
            gc_test_access::scan()->back_treadmill(),
            cell);
}
int assert_scan(gc::heap::cell* cell)
{
    return assert_in_list(
            gc_test_access::scan(),
            gc_test_access::free()->back_treadmill(),
            cell);
}
int assert_free(gc::heap::cell* cell)
{
    return assert_in_list(
            gc_test_access::free(),
            gc_test_access::bottom()->back_treadmill(),
            cell);
}

#endif
