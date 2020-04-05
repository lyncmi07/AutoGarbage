CC=g++
LDFLAGS=-Wall
CCFLAGS=-Wall -std=c++17 -Igclib/include

TARGET=a.out

all=$(TARGET)

$(TARGET): obj/main.o
	$(CC) $(LDFLAGS) -o $@ $<

obj/main.o: src/main.cpp gclib/include/gc.h

	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f obj/*.o $(TARGET)
