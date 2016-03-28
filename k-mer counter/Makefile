CC = g++
CFLAGS = -O3 -std=c++11 -Wall -L/usr/local/lib -I/usr/local/include
TESTFLAGS = $(CFLAGS)

CPP = $(filter-out main.cpp, $(filter-out test.cpp, $(filter-out $(TSTSRC), $(wildcard *.cpp))))
OBJ = $(CPP:.cpp=.o)

TSTSRC = $(wildcard *_test.cpp)
TSTOBJ = $(TSTSRC:.cpp=.o)

HDR = $(filter-out catch.hpp, $(wildcard *.h))

count: $(OBJ) main.o
	$(CC) $(CFLAGS) -o $@ $^

test: $(OBJ) $(TSTOBJ) test.o
	$(CC) $(TESTFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

test.o: test.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -r *.o test count deps.mk

depend:
	$(CC) $(CFLAGS) -MM $(CPP) $(TSTSRC) > deps.mk

-include deps.mk
