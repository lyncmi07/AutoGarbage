#include "gc_object.h"
#include "gc_cell.h"
#include "gc_heap.h"

void gc::object::gc_white()
{
    _mark = 'W';
    gc::heap::heap_struct::get()->link_scan(this);
}

void gc::object::gc_grey()
{
    _mark = 'G';
    gc::heap::heap_struct::get()->link_top(this);
    _current_grey_objects++;
}

void gc::object::gc_black()
{
    _mark = 'B';
    gc::heap::heap_struct::get()->link_bottom(this);
    _current_grey_objects--;
    gc_grey_fields();
}

void gc::object::gc_grey_fields()
{
    gc::field<gc::object>* field_ptr = (gc::field<gc::object>*) (((unsigned long) this) + sizeof(gc::object));
    bool* fields_end_ptr = gc_fields_end();

    while ((bool*)field_ptr < fields_end_ptr)
    {
        field_ptr->gc_mark();
	field_ptr = (gc::field<gc::object>*) (((unsigned long) field_ptr) + sizeof(gc::field<gc::object>));
    }
}

bool* gc::object::gc_fields_end() { return nullptr; }


gc::object::object():
    cell(nullptr, nullptr, _size),
    _mark('W'),
    _iteration(gc::heap::heap_struct::get()->odd_iteration())
{
    gc::heap::heap_struct::get()->link_scan(this);
}

void* gc::object::operator new(size_t size)
{
    void* p = gc::heap::heap_struct::get()->malloc(size);
    ((object*) p)->_size = size;

    return p;
}

size_t gc::object::size()
{
    return _size;
}

void gc::object::gc_mark()
{
    switch (_mark) //seg fault
    {
        case 'B':
            if (_iteration == gc::heap::heap_struct::get()->odd_iteration()) return;
            break;
        case 'G':
            gc_black();
            return;
        case 'W':
            gc_grey();
	    return;
    }

    //Move to next iteration:
    gc_grey();
}

void gc::object::debug_fields()
{
    std::cout << "The object starts at: " << this << std::endl;
    std::cout << "The fields end at: " << ((unsigned long)gc_fields_end()) - ((unsigned long)this) << std::endl;
}

void gc::object::debug_gc()
{
    std::cout << "Marked: " << _mark << std::endl;
}
