CC=g++
LDFLAGS=-Wall
LDLIBS=-Lgclib/lib -lgc
CCFLAGS=-Wall -std=c++17 -Igclib/include -g

TARGET=a.out

all=$(TARGET)

$(TARGET): obj/main.o gclib/lib/libgc.a gclib/include/gc.h.gch
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

obj/main.o: src/main.cpp gclib/include/*.h
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/include/gc.h.gch: gclib/include/*.h
	$(CC) $(CCFLAGS) -c $<

gclib/lib/libgc.a: gclib/obj/gc_statics.o gclib/obj/gc_object.o gclib/obj/gc_free_cell.o gclib/obj/gc_cell.o gclib/obj/gc_heap.o
	ar ru $@ $^
	ranlib $@

gclib/obj/gc_statics.o: gclib/src/gc_statics.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<
gclib/obj/gc_object.o: gclib/src/gc_object.cpp gclib/include/gc_object.h
	$(CC) $(CCFLAGS) -o $@ -c $<
gclib/obj/gc_free_cell.o: gclib/src/gc_free_cell.cpp gclib/include/gc_free_cell.h
	$(CC) $(CCFLAGS) -o $@ -c $<
gclib/obj/gc_cell.o: gclib/src/gc_cell.cpp gclib/include/gc_cell.h
	$(CC) $(CCFLAGS) -o $@ -c $<
gclib/obj/gc_heap.o: gclib/src/gc_heap.cpp gclib/include/gc_heap.h
	$(CC) $(CCFLAGS) -o $@ -c $<


clean:
	rm -f obj/*.o $(TARGET) gclib/include/*.gch gclib/obj/*.o gclib/lib/*.a
