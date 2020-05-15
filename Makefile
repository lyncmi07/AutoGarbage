.PHONY: test clean
CC=g++
LDFLAGS=-Wall
LDLIBS=-Lgclib/lib -lgc
CCFLAGS=-Wall -std=c++17 -Igclib/include -g

LIB_SOURCES=$(wildcard gclib/src/*.cpp)
LIB_OBJECTS=$(addprefix gclib/obj/, $(notdir $(LIB_SOURCES:.cpp=.o)))

CCFLAGS += -DPERFORMANCE_TIMERS

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

test: test_build
	make test_build
	./test_build

test_build: obj/test.o gclib/lib/libgc.a 
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

obj/test.o: src/tests/RunTests.cpp src/tests/*.h
	$(CC) $(CCFLAGS) -o $@ -c $<

performance_test: test_performance_build
	make test_performance_build
	./test_performance_build

test_performance_build: obj/test_performance.o gclib/lib/libgc.a
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

obj/test_performance.o: src/tests/performance.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f obj/*.o $(TARGET) gclib/include/*.gch gclib/obj/*.o gclib/lib/*.a
