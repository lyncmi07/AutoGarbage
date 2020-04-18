CC=g++
LDFLAGS=-Wall
LDLIBS=-Lgclib/lib -lgc
CCFLAGS=-Wall -std=c++17 -Igclib/include -g

LIB_SOURCES=$(wildcard gclib/src/*.cpp)
LIB_OBJECTS=$(addprefix gclib/obj/, $(notdir $(LIB_SOURCES:.cpp=.o)))

TARGET=a.out

all=$(TARGET)

$(TARGET): obj/main.o gclib/lib/libgc.a gclib/include/gc.h
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

obj/main.o: src/main.cpp gclib/include/*.h
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/lib/libgc.a: $(LIB_OBJECTS)
	ar ru $@ $^
	ranlib $@

gclib/obj/%.o: gclib/src/%.cpp gclib/include/%.h
	$(CC) $(CCFLAGS) -o $@ -c $<

gclib/obj/%.o: gclib/src/%.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f obj/*.o $(TARGET) gclib/include/*.gch gclib/obj/*.o gclib/lib/*.a
