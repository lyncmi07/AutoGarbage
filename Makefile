CC=g++
LDFLAGS=-Wall -Lgclib/lib -lgc
CCFLAGS=-Wall -std=c++17 -Igclib/include

TARGET=a.out

all=$(TARGET)

$(TARGET): obj/main.o gclib/lib/libgc.a
	$(CC) $(LDFLAGS) -o $@ $<

obj/main.o: src/main.cpp gclib/include/*.h
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/lib/libgc.a: gclib/obj/gc_statics.o gclib/obj/gc_functions.o
	ar ru $@ $^
	ranlib $@

gclib/obj/gc_statics.o: gclib/src/gc_statics.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/obj/gc_functions.o: gclib/src/gc_functions.cpp gclib/include/gc_statics.h
	$(CC) $(CCFLAGS) -o $@ -c $<


clean:
	rm -f obj/*.o $(TARGET) gclib/obj/*.o gclib/lib/*.a
