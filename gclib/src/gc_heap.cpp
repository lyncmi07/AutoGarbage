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
    _end_gc_fields_magic_ptr(((char*)_heap_space) - 1),
    _bottom(new gc::heap::cell(nullptr, nullptr, 0, false, 0)),
    _top(new gc::heap::cell(nullptr, nullptr, 0, false, 0)),
    _scan(new gc::heap::cell(nullptr, nullptr, 0, false, 0)),
    _free(new gc::heap::cell(nullptr, nullptr, 0, false, 0)),
    _static_objects_start_ptr(nullptr),
    _gc_iteration(0)
{
    gc::heap::cell* initial_free_cell = (gc::heap::cell*) _heap_space;
    (*initial_free_cell) = gc::heap::cell(nullptr, nullptr, heap_size, false, 0);

    _bottom->fwd_link_treadmill(_top);
    _top->fwd_link_treadmill(_scan);
    _scan->fwd_link_treadmill(_free);
    _free->fwd_link_treadmill(initial_free_cell);
    initial_free_cell->fwd_link_treadmill(_bottom);

    initial_free_cell->back_link_location(nullptr);
    initial_free_cell->fwd_link_location(nullptr);

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

            return next_free_cell;
        }
        else if (next_free_cell->size() == size)
        {
            next_free_cell->unlink_treadmill();

            return next_free_cell;
        }
        else if(next_free_cell->size() > current_largest_cell->size())
        {
            current_largest_cell = next_free_cell;
        }
        else
        {
            //Cell is not large enough. Can it be merged with the next cell?

            /*if (next_free_cell->mergable_with_fwd_treadmill())
            {
                //Merge cells and try again
                next_free_cell->merge_with_fwd_treadmill();

            }
            else
            {
                //Unable to merge cells, try next cell
                next_free_cell = next_free_cell->fwd_treadmill();
            }*/

            if (next_free_cell->fwd_location() != nullptr && next_free_cell->fwd_location()->garunteed_free() && next_free_cell->mergable_with_fwd_location())
            {
                //Merge cells and try again
                next_free_cell->fwd_location()->unlink_treadmill();
                next_free_cell->merge_with_fwd_location();

            }
            else
            {
                //Unable to merge cells, try next cell
                next_free_cell = next_free_cell->fwd_treadmill();
            }

        }


    }

    return nullptr;
}

void gc::heap::heap_struct::add_to_initialization_list(gc::object* object)
{
    gc::object* current_start = _initialization_objects_start_ptr;

    _initialization_objects_start_ptr = object;
    object->fwd_link_treadmill(current_start);
    object->back_link_treadmill(nullptr);
}

void gc::heap::heap_struct::remove_from_initialization_list(gc::object* object)
{
    gc::object* fwd_ptr = object->fwd_object();
    object->unlink_treadmill();
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
    size_t actual_size_allocated = allocation_current_cell->size();
    gc::heap::cell* back_location = allocation_current_cell->back_location();
    gc::heap::cell* fwd_location = allocation_current_cell->fwd_location();
    gc::object* positioned_object = (gc::object*) allocation_current_cell->actual_position();

    positioned_object->set_size(actual_size_allocated);
    positioned_object->back_link_location(back_location);
    positioned_object->fwd_link_location(fwd_location);

    add_to_initialization_list(positioned_object);
    return positioned_object;
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

    size_t total_heap_size_found = 0;

    for (unsigned int ptr_index = 0; ptr_index < 4; ptr_index++)
    {
        std::cout << ptr_names[ptr_index] << ":" << heap_ptrs[ptr_index] << " -> " << std::endl;
        gc::heap::cell* current_ptr = heap_ptrs[ptr_index]->fwd_treadmill();
        gc::heap::cell* last_ptr = heap_ptrs[ptr_index+1];

        while (current_ptr != last_ptr)
        {
            std::cout << "[" << current_ptr << "(" << current_ptr->actual_position() << "):" << current_ptr->size() << "(";

            total_heap_size_found += current_ptr->size();

            std::cout <<
                ((current_ptr->fwd_location() != nullptr)
                 ? ((current_ptr->back_location() != nullptr)
                     ? "X)] ->"
                     : ">)] ->")
                 : ((current_ptr->back_location() != nullptr)
                     ? "<)] ->"
                     : " )] ->"))
                << std::endl;

            current_ptr = current_ptr->fwd_treadmill();
        }
    }

    std::cout << "bottom }" << std::endl;
    std::cout << "Heap space found: " << total_heap_size_found << " bytes" << std::endl;
}

void gc::heap::heap_struct::make_black(gc::object* obj)
{
    obj->unlink_treadmill();
    obj->fwd_link_treadmill(_bottom->fwd_treadmill());
    obj->back_link_treadmill(_bottom);
}

void gc::heap::heap_struct::make_grey(gc::object* obj)
{
    obj->unlink_treadmill();
    obj->back_link_treadmill(_scan->back_treadmill());
    obj->fwd_link_treadmill(_scan);
}

void gc::heap::heap_struct::make_ecru(gc::object* obj)
{
    obj->unlink_treadmill();
    obj->back_link_treadmill(_free->back_treadmill());
    obj->fwd_link_treadmill(_free);
}

void gc::heap::heap_struct::make_white(gc::heap::cell* obj)
{
    obj->unlink_treadmill();
    obj->back_link_treadmill(_bottom->back_treadmill());
    obj->fwd_link_treadmill(_bottom);
}

void gc::heap::heap_struct::replace_free_start(gc::heap::cell* obj)
{
    obj->unlink_treadmill();
    obj->fwd_link_treadmill(_free->fwd_treadmill());
    _free->fwd_link_treadmill(obj);
}

void gc::heap::heap_struct::flip()
{
    flip_list();
    _gc_iteration++;
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
    gc::heap::cell* current_cell(_top->fwd_treadmill());

    while(current_cell != _scan)
    {
        gc::object* current_object = (gc::object*) current_cell;
        current_object->gc_mark();
        current_cell = _top->fwd_treadmill();
    }
}

gc::object* gc::heap::heap_struct::bottom()
{
    return (gc::object*) _bottom->fwd_treadmill();
}

gc::object* gc::heap::heap_struct::top()
{
    return (gc::object*) _top->fwd_treadmill();
}

gc::object* gc::heap::heap_struct::scan()
{
    return (gc::object*) _scan->fwd_treadmill();
}

gc::heap::cell* gc::heap::heap_struct::free()
{
    return _free->fwd_treadmill();
}


void gc::heap::heap_struct::flip_list()
{
    {
        //white to free: _scan->f->l->_free->e => _scan->_free->f->l->e
        auto first_scan_cell = _scan->fwd_treadmill(); //f
        auto last_scan_cell = _free->back_treadmill(); //l
        auto first_free_cell = _free->fwd_treadmill(); //e

        if (first_scan_cell != _free)
        {
            // _scan->_free
            _scan->fwd_link_treadmill(_free);

            // l->e
            last_scan_cell->fwd_link_treadmill(first_free_cell);
    
            // _free->f
            _free->fwd_link_treadmill(first_scan_cell);
        }
    }
    {
        // black to white: _bottom->f->l->_top->_scan->_free => _bottom->_top->_scan->f->l->_free
        auto first_bottom_cell = _bottom->fwd_treadmill(); //f
        auto last_bottom_cell = _top->back_treadmill(); //l

        if (first_bottom_cell != _top)
        {
            // _bottom->_top
            _bottom->fwd_link_treadmill(_top);

            // _scan->f
            _scan->fwd_link_treadmill(first_bottom_cell);

            // l->_free
            last_bottom_cell->fwd_link_treadmill(_free);
        }
    }
}

void gc::heap::heap_struct::add_fragment_memory(void* position, size_t size)
{
    _fragment_size += size;
    _fragment_memory_list.push_back(new fragment_memory(position, size));
}
