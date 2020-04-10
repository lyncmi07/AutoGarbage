CC=g++
LDFLAGS=-Wall -Lgclib/lib -lgc
CCFLAGS=-Wall -std=c++17 -Igclib/include -g

TARGET=a.out

all=$(TARGET)

$(TARGET): obj/main.o gclib/lib/libgc.a gclib/include/gc.h.gch
	$(CC) $(LDFLAGS) -o $@ $<

obj/main.o: src/main.cpp gclib/include/*.h
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/include/gc.h.gch: gclib/include/*.h
	$(CC) $(CCFLAGS) -c $<

gclib/lib/libgc.a: gclib/obj/gc_statics.o gclib/obj/gc_functions.o
	ar ru $@ $^
	ranlib $@

gclib/obj/gc_statics.o: gclib/src/gc_statics.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/obj/gc_functions.o: gclib/src/gc_functions.cpp gclib/include/gc_statics.h
	$(CC) $(CCFLAGS) -o $@ -c $<


clean:
	rm -f obj/*.o $(TARGET) gclib/include/*.gch gclib/obj/*.o gclib/lib/*.a
