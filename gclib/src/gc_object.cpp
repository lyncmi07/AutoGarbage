#include "gc_object.h"
#include "gc_cell.h"
#include "gc_heap.h"
#include "gc_field.h"

void gc::object::gc_ecru()
{
    _mark = 'W';
    gc::heap::heap_struct::get()->make_ecru(this);
}

void gc::object::gc_grey()
{
    _mark = 'G';
    gc::heap::heap_struct::get()->make_grey(this);
    _current_grey_objects++;
}

void gc::object::gc_black()
{
    _mark = 'B';
    gc::heap::heap_struct::get()->make_black(this);
    _current_grey_objects--;
    gc_grey_fields();
}

void gc::object::gc_grey_fields()
{
    //Move to the end of gc::object to get to memory formed by extended class
    gc::field<gc::object>* field_ptr = (gc::field<gc::object>*) (((unsigned long) this) + sizeof(gc::object));

    while (*((void**)field_ptr) != heap::heap_struct::get()->end_gc_fields_magic_ptr())
    {
        field_ptr->gc_mark();
	    field_ptr = (gc::field<gc::object>*) (((unsigned long) field_ptr) + sizeof(gc::field<gc::object>));
    }
}

bool* gc::object::gc_fields_end() { return nullptr; }


gc::object::object():
    cell(true, gc::heap::heap_struct::get()->gc_iteration()),
    _mark('I')
{
}

void* gc::object::operator new(size_t size)
{
    void* p = gc::heap::heap_struct::get()->malloc(size);

    return p;
}

char gc::object::current_mark()
{
    return _mark;
}

void gc::object::gc_mark()
{
    switch (_mark) //seg fault
    {
        case 'B':
            if (!iteration_stale())
            {
                return;
            }
            else
            {
                //Move to next iteration:
                gc_grey();
                update_iteration();
                return;
            }
        case 'G':
            gc_black();
            return;
        case 'W':
            gc_grey();
            return;
        case 'I':
            gc_ecru();
	        return;
    }

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
