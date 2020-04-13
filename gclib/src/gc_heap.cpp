#include "gc_heap.h"

#include "gc_static_ptr.h"
#include "gc_free_cell.h"
#include "gc_object.h"
#include <new>
#include <iostream>

gc::heap::heap_struct* gc::heap::heap_struct::INSTANCE = nullptr;

gc::heap::heap_struct::heap_struct(size_t heap_size):
    _heap_size(heap_size),
    _heap_space((void*) new char[_heap_size]),
    _bottom(nullptr),
    _bottom_initial(nullptr),
    _top(nullptr),
    _scan(nullptr),
    _scan_initial(nullptr),
    _static_objects_start_ptr(nullptr),
    _odd_iteration(true)
{
    gc::heap::free_cell* initial_free_cell = (gc::heap::free_cell*) _heap_space;
    (*initial_free_cell) = gc::heap::free_cell(nullptr, heap_size);

    _free = initial_free_cell;
}

void* gc::heap::heap_struct::malloc(size_t size)
{
    gc::heap::free_cell* next_free_cell = _free;
    gc::heap::free_cell* current_largest_cell = _free;

    while(next_free_cell != nullptr)
    {
        if (next_free_cell->size() > size)
        {
            gc::heap::free_cell* resized_cell = next_free_cell->resize(size);
            if (next_free_cell == current_largest_cell || resized_cell->size() >= current_largest_cell->size())
            {
                _free = resized_cell;
            }
            else
            {
                _free = current_largest_cell;
            }

            return next_free_cell;
        }
        else if (next_free_cell ->size() == size)
        {
            _free = current_largest_cell;
            return next_free_cell;
        }
        else if(next_free_cell-> size() > current_largest_cell->size())
        {
            current_largest_cell = next_free_cell;
        }

        next_free_cell = next_free_cell->fwd_free_cell();
    }

    throw std::bad_alloc(); //TODO: Could improve by performing gc and trying to allocate again
}

void gc::heap::heap_struct::add_static(void* sptr)
{
    gc::static_ptr<bool>* sptr_typed = (gc::static_ptr<bool>*) sptr;
    sptr_typed->_back_static_object = nullptr;
    sptr_typed->_fwd_static_object = _static_objects_start_ptr;

    if(_static_objects_start_ptr != nullptr)
    {
        _static_objects_start_ptr->_back_static_object = sptr_typed;
    }

    _static_objects_start_ptr = sptr_typed;
}

void gc::heap::heap_struct::remove_static(void* sptr)
{
    gc::static_ptr<bool>* sptr_typed = (gc::static_ptr<bool>*) sptr;

    gc::static_ptr<bool>* back_ptr = sptr_typed->_back_static_object;
    gc::static_ptr<bool>* fwd_ptr = sptr_typed->_fwd_static_object;

    if (back_ptr == nullptr)
    {
        _static_objects_start_ptr = fwd_ptr;
    }
    else
    {
        back_ptr->_fwd_static_object = fwd_ptr;
    }

    if (fwd_ptr != nullptr)
    {
        fwd_ptr->_back_static_object = back_ptr;
    }
}

void gc::heap::heap_struct::print_gc_debug()
{
    std::cout << "--BEGIN GC DEBUG--" << std::endl;

    print_static_objects_list();

    std::cout << std::endl;

    print_heap_pointers();

    std::cout << "--END GC DEBUG--" << std::endl;
}

void gc::heap::heap_struct::print_static_objects_list()
{
    std::cout << "static_objects list: { start_ptr -> ";

    gc::static_ptr<bool>* current_ptr = gc::heap::heap_struct::get()->static_objects_start_ptr();

    while (current_ptr != nullptr)
    {
        std::cout << current_ptr << " -> ";
        current_ptr = current_ptr->_fwd_static_object;
    }

    std::cout << "nullptr }" << std::endl;
}

void gc::heap::heap_struct::print_heap_pointers()
{
    std::cout << "heap pointers: { " << std::endl;

    std::string ptr_names[] = { "bottom", "top", "scan", "free" };

    gc::heap::heap_struct* heap_instance = gc::heap::heap_struct::get();
    gc::heap::cell* heap_ptrs[] = {
        heap_instance->bottom(),
        heap_instance->top(),
        heap_instance->scan(),
        heap_instance->free()
    };

    for (unsigned int ptr_index = 0; ptr_index < 4; ptr_index++)
    {
        std::cout << ptr_names[ptr_index] << " -> " << std::endl;
        gc::heap::cell* current_ptr = heap_ptrs[ptr_index];

        while (current_ptr != nullptr)
        {
            std::cout << "[" << current_ptr << ":" << current_ptr->size() << "] -> " << std::endl;
            current_ptr = current_ptr->fwd_cell();
        }
    }

    std::cout << "bottom }" << std::endl;
}
