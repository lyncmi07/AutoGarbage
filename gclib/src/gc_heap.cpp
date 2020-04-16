#include "gc_heap.h"

#include "gc_static_ptr.h"
#include "gc_free_cell.h"
#include "gc_object.h"
#include "gc_cell.h"
#include <new>
#include <iostream>

gc::heap::heap_struct* gc::heap::heap_struct::INSTANCE = nullptr;

gc::heap::heap_struct::heap_struct(size_t heap_size):
    _heap_size(heap_size),
    _heap_space((void*) new char[_heap_size]),
    _bottom(new gc::heap::cell(nullptr, nullptr, 0)),
    _top(new gc::heap::cell(nullptr, nullptr, 0)),
    _scan(new gc::heap::cell(nullptr, nullptr, 0)),
    _free(new gc::heap::cell(nullptr, nullptr, 0)),
    _static_objects_start_ptr(nullptr),
    _odd_iteration(true)
{
    gc::heap::free_cell* initial_free_cell = (gc::heap::free_cell*) _heap_space;
    (*initial_free_cell) = gc::heap::free_cell(nullptr, nullptr, heap_size);

    // Initial list set up
    _bottom->fwd_link(_top);
    _top->back_link(_bottom);
    _top->fwd_link(_scan);
    _scan->back_link(_top);
    _scan->fwd_link(_free);
    _free->back_link(_scan);
    _free->fwd_link(initial_free_cell);
    initial_free_cell->back_link(_free);
    initial_free_cell->fwd_link(_bottom);
}

void* gc::heap::heap_struct::malloc(size_t size)
{
    gc::heap::free_cell* next_free_cell = free();
    gc::heap::free_cell* current_largest_cell = free();

    while(next_free_cell != nullptr)
    {
        if (next_free_cell->size() > size)
        {
            gc::heap::free_cell* resized_cell = next_free_cell->resize(size);
            if (next_free_cell == current_largest_cell || resized_cell->size() >= current_largest_cell->size())
            {
                //TODO: This is wrong. Whatever pointed to the original cell will still be pointing to it. It needs to move on to the next cell.
                replace_free_start(resized_cell);
            }
            else
            {
                replace_free_start(current_largest_cell);
            }


            next_free_cell->fwd_link(nullptr);
            next_free_cell->back_link(nullptr);
            return next_free_cell;
        }
        else if (next_free_cell->size() == size)
        {
            //TODO: unlink next_free_cell, safely move current_largest_cell to front

            next_free_cell->unlink();
            if (next_free_cell != current_largest_cell) replace_free_start(current_largest_cell);

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
    gc::static_ptr<gc::object>* sptr_typed = (gc::static_ptr<gc::object>*) sptr;
    sptr_typed->_back_static_object = nullptr;
    sptr_typed->_fwd_static_object = _static_objects_start_ptr;

    if(_static_objects_start_ptr != nullptr)
    {
        _static_objects_start_ptr->_back_static_object = sptr_typed;
    }

    _static_objects_start_ptr = sptr_typed;

    sptr_typed->gc_mark();
}

void gc::heap::heap_struct::remove_static(void* sptr)
{
    gc::static_ptr<gc::object>* sptr_typed = (gc::static_ptr<gc::object>*) sptr;

    gc::static_ptr<gc::object>* back_ptr = sptr_typed->_back_static_object;
    gc::static_ptr<gc::object>* fwd_ptr = sptr_typed->_fwd_static_object;

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

    gc::static_ptr<gc::object>* current_ptr = gc::heap::heap_struct::get()->static_objects_start_ptr();

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
        _bottom,
        _top,
        _scan,
        _free,
        _bottom
    };

    for (unsigned int ptr_index = 0; ptr_index < 4; ptr_index++)
    {
        std::cout << ptr_names[ptr_index] << ":" << heap_ptrs[ptr_index] << " -> " << std::endl;
        gc::heap::cell* current_ptr = heap_ptrs[ptr_index]->fwd_cell();
        gc::heap::cell* last_ptr = heap_ptrs[ptr_index+1];

        while (current_ptr != last_ptr)
        {
            std::cout << "[" << current_ptr << ":" << current_ptr->size() << "] -> " << std::endl;
            current_ptr = current_ptr->fwd_cell();
        }
    }

    std::cout << "bottom }" << std::endl;
}

void gc::heap::heap_struct::link_bottom(gc::object* obj)
{
    obj->unlink();
    obj->fwd_link(_bottom->fwd_cell());
    obj->fwd_cell()->back_link(obj);
    _bottom->fwd_link(obj);
    obj->back_link(_bottom);
}

void gc::heap::heap_struct::link_top(gc::object* obj)
{
    obj->unlink();
    obj->fwd_link(_top->fwd_cell());
    obj->fwd_cell()->back_link(obj);
    _top->fwd_link(obj);
    obj->back_link(_top);
}

void gc::heap::heap_struct::link_scan(gc::object* obj)
{
    obj->unlink();
    obj->fwd_link(_scan->fwd_cell());
    obj->fwd_cell()->back_link(obj);
    _scan->fwd_link(obj);
    obj->back_link(_scan);
}

void gc::heap::heap_struct::link_free(gc::heap::free_cell* obj)
{
    obj->unlink();
    obj->fwd_link(_free->fwd_cell());
    obj->fwd_cell()->back_link(obj);
    _free->fwd_link(obj);
    obj->back_link(_free);
}

void gc::heap::heap_struct::replace_free_start(gc::heap::free_cell* obj)
{
    obj->unlink();
    obj->fwd_link(_free->fwd_cell());
    obj->fwd_cell()->back_link(obj);
    _free->fwd_link(obj);
    obj->back_link(_free);
}

void gc::heap::heap_struct::flip()
{
    flip_list();
    _odd_iteration = !_odd_iteration;
}

void gc::heap::heap_struct::flip_list()
{
    {
        //white to free: _scan->f->l->_free->e => _scan->_free->f->l->e
        auto first_scan_cell = _scan->fwd_cell(); //f
        auto last_scan_cell = _free->back_cell(); //l
        auto first_free_cell = _free->fwd_cell(); //e

        if (first_scan_cell != _free)
        {
            // _scan->_free
            _scan->fwd_link(_free);
            _free->back_link(_scan);

            // l->e
            last_scan_cell->fwd_link(first_free_cell);
            first_free_cell->back_link(last_scan_cell);
    
            // _free->f
            _free->fwd_link(first_scan_cell);
            first_scan_cell->back_link(_free);
        }
    }
    {
        // black to white: _bottom->f->l->_top->_scan->_free => _bottom->_top->_scan->f->l->_free
        auto first_bottom_cell = _bottom->fwd_cell(); //f
        auto last_bottom_cell = _top->back_cell(); //l

        if (first_bottom_cell != _top)
        {
            // _bottom->_top
            _bottom->fwd_link(_top);
            _top->back_link(_bottom);

            // _scan->f
            _scan->fwd_link(first_bottom_cell);
            first_bottom_cell->back_link(_scan);

            // l->_free
            last_bottom_cell->fwd_link(_free);
            _free->back_link(last_bottom_cell);
        }
    }
}
