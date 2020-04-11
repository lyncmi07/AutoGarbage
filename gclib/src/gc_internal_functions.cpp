#include <iostream>

#include "gc_internal_functions.h"
#include "gc_static_ptr.h"
#include "gc_statics.h"
#include "gc_object.h"


void gc::add_static(void* sptr)
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

void gc::remove_static(void* sptr)
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

void print_static_objects_list();
void print_heap_pointers();
void gc::print_gc_debug()
{
    std::cout << "--BEGIN GC DEBUG--" << std::endl;

    print_static_objects_list();

    std::cout << std::endl;

    print_heap_pointers();

    std::cout << "--END GC DEBUG--" << std::endl;
}

void print_static_objects_list()
{
    std::cout << "static_objects list: { start_ptr -> ";

    gc::static_ptr<bool>* current_ptr = gc::_static_objects_start_ptr;

    while (current_ptr != nullptr)
    {
        std::cout << current_ptr << " -> ";
        current_ptr = current_ptr->_fwd_static_object;
    }

    std::cout << "nullptr }" << std::endl;
}

void print_heap_pointers()
{
    std::cout << "heap pointers: { " << std::endl;

    std::string ptr_names[] = { "bottom", "top", "scan", "free" };
    gc::object* heap_ptrs[] = { gc::heap::_bottom, gc::heap::_top, gc::heap::_scan, gc::heap::_free };

    for (unsigned int ptr_index = 0; ptr_index < 4; ptr_index++)
    {
        std::cout << ptr_names[ptr_index] << " -> " << std::endl;
        gc::object* current_ptr = heap_ptrs[ptr_index];

        while (current_ptr != nullptr)
        {
            std::cout << "[" << current_ptr << ":" << current_ptr->size() << "] -> " << std::endl;
        }
    }

    std::cout << "bottom }" << std::endl;
}
