#include "gc_heap.h"

#include "gc_static_ptr.h"
#include "gc_object.h"
#include "gc_cell.h"
#include <new>
#include <iostream>
#include <variant>

gc::heap::heap_struct* gc::heap::heap_struct::INSTANCE = nullptr;

gc::heap::fragment_memory::fragment_memory(void* _fragment_position, size_t _size): fragment_position(_fragment_position), size(_size)
{
}

gc::heap::heap_struct::heap_struct(size_t heap_size):
    _heap_size(heap_size),
    _heap_space((void*) new char[_heap_size]),
    _bottom(new gc::heap::cell(nullptr, nullptr, 0, false)),
    _top(new gc::heap::cell(nullptr, nullptr, 0, false)),
    _scan(new gc::heap::cell(nullptr, nullptr, 0, false)),
    _free(new gc::heap::cell(nullptr, nullptr, 0, false)),
    _static_objects_start_ptr(nullptr),
    _odd_iteration(true)
{
    gc::heap::cell* initial_free_cell = (gc::heap::cell*) _heap_space;
    (*initial_free_cell) = gc::heap::cell(nullptr, nullptr, heap_size, false);

    _bottom->fwd_link(_top);
    _top->back_link(_bottom);
    _top->fwd_link(_scan);
    _scan->back_link(_top);
    _scan->fwd_link(_free);
    _free->back_link(_scan);
    _free->fwd_link(initial_free_cell);
    initial_free_cell->back_link(_free);
    initial_free_cell->fwd_link(_bottom);

    _garbage_collection_cycle = 0;
}

gc::heap::heap_struct::~heap_struct()
{
    delete _heap_space;
    delete _bottom;
    delete _top;
    delete _scan;
    delete _free;
    delete _static_objects_start_ptr;
}

gc::heap::cell* gc::heap::heap_struct::attempt_allocate(size_t size)
{
    gc::heap::cell* next_free_cell = free();
    gc::heap::cell* current_largest_cell = free();

    while(next_free_cell != _bottom)
    {
        if (next_free_cell->size() > size)
        {
            gc::heap::cell* resized_cell = next_free_cell->resize(size);


            if (resized_cell != nullptr && !(next_free_cell == current_largest_cell || resized_cell->size() >= current_largest_cell->size()))
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

        next_free_cell = next_free_cell->fwd_cell();
    }

    return nullptr;
}

void gc::heap::heap_struct::add_to_initialization_list(gc::object* object)
{
    gc::object* current_start = _initialization_objects_start_ptr;

    _initialization_objects_start_ptr = object;
    object->fwd_link(current_start);
    object->back_link(nullptr);

    if (current_start != nullptr)
    {
        current_start->back_link(object);
    }
}

void gc::heap::heap_struct::remove_from_initialization_list(gc::object* object)
{
    gc::object* fwd_ptr = object->fwd_object();
    object->unlink();
    if (_initialization_objects_start_ptr == object)
    {
        _initialization_objects_start_ptr = fwd_ptr;
    }

    //Take object out of initialization
    object->gc_mark();
}

void* gc::heap::heap_struct::malloc(size_t size)
{
    gc::heap::cell* allocation_current_cell = attempt_allocate(size);
    if (allocation_current_cell != nullptr) goto return_allocation;

    collect_garbage(); //free up memory

    allocation_current_cell = attempt_allocate(size);
    if (allocation_current_cell != nullptr) goto return_allocation;

    collect_garbage(); //last free up memory attempt in case black objects became white

    allocation_current_cell = attempt_allocate(size);
    if (allocation_current_cell != nullptr) goto return_allocation;

    throw std::bad_alloc();

return_allocation:
    add_to_initialization_list((gc::object*) allocation_current_cell->actual_position());
    return allocation_current_cell->actual_position();
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
    print_gc_info();

    print_static_objects_list();

    std::cout << std::endl;

    print_initialization_objects_list();

    std::cout << std::endl;

    print_heap_pointers();

    std::cout << "--END GC DEBUG--" << std::endl;
}

void gc::heap::heap_struct::print_gc_info()
{
    std::cout << "gc_cycle:" << _garbage_collection_cycle << " lost_cells:" << _fragment_memory_list.size() << " lost_size:" << _fragment_size << std::endl;
}

void gc::heap::heap_struct::print_initialization_objects_list()
{
    std::cout << "initalization_objects list: { start_ptr -> ";

    gc::object* current_ptr = _initialization_objects_start_ptr;

    while (current_ptr != nullptr)
    {
        std::cout << current_ptr << " -> ";
        current_ptr = current_ptr->fwd_object();
    }

    std::cout << "nullptr }" << std::endl;
}

void gc::heap::heap_struct::print_static_objects_list()
{
    std::cout << "static_objects list: { start_ptr -> ";

    gc::static_ptr<gc::object>* current_ptr = _static_objects_start_ptr;

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
            std::cout << "[" << current_ptr << "(" << current_ptr->actual_position() << "):" << current_ptr->size() << "(";

            //Check contiguous with next cell
            void* contiguous_position = (void*)((char*)current_ptr->actual_position()) + current_ptr->size();
            std::cout <<
                ((contiguous_position == (void*)((char*)current_ptr->back_cell()->actual_position()))
                    ? "C)] ->"
                    : " )] ->")
                << std::endl;

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

void gc::heap::heap_struct::link_free(gc::heap::cell* obj)
{
    obj->unlink();
    obj->fwd_link(_free->fwd_cell());
    obj->fwd_cell()->back_link(obj);
    _free->fwd_link(obj);
    obj->back_link(_free);
}

void gc::heap::heap_struct::replace_free_start(gc::heap::cell* obj)
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

void gc::heap::heap_struct::collect_garbage()
{
    _garbage_collection_cycle++;
    ungrey_all();
    flip();
    grey_static_ptrs();
}

void gc::heap::heap_struct::grey_static_ptrs()
{
    gc::static_ptr<gc::object>* current_static_object = _static_objects_start_ptr;

    while(current_static_object != nullptr)
    {
        current_static_object->gc_mark();
        current_static_object = current_static_object->_fwd_static_object;
    }

}

void gc::heap::heap_struct::ungrey_all()
{
    gc::heap::cell* current_cell(_scan->back_cell());

    while(current_cell != _top)
    {
        gc::object* current_object = (gc::object*) current_cell;
        current_object->gc_mark();
        current_cell = _scan->back_cell();
    }
}

gc::object* gc::heap::heap_struct::bottom()
{
    return (gc::object*) _bottom->fwd_cell();
}

gc::object* gc::heap::heap_struct::top()
{
    return (gc::object*) _top->fwd_cell();
}

gc::object* gc::heap::heap_struct::scan()
{
    return (gc::object*) _scan->fwd_cell();
}
void gc::heap::heap_struct::set_scan(gc::object* new_scan)
{
    _scan->fwd_link(new_scan);
}

gc::heap::cell* gc::heap::heap_struct::free()
{
    return _free->fwd_cell();
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

void gc::heap::heap_struct::add_fragment_memory(void* position, size_t size)
{
    _fragment_size += size;
    _fragment_memory_list.push_back(new fragment_memory(position, size));
}
