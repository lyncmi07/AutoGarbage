#ifndef GC_LIB_H
#define GC_LIB_H

#include <unordered_set>

#include "gc_object.h"
#include "gc_statics.h"
#include "gc_field.h"
#include "gc_static_ptr.h"

#define END_GC_FIELDS bool _gc_fields_end; bool* gc_fields_end() override { return &_gc_fields_end; };

#endif
