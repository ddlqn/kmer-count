CC = g++
CFLAGS = -O3 -std=c++11 -Wall -I/usr/local/include
LFLAGS = $(CFLAGS) -L/usr/local/lib
TESTFLAGS = $(CFLAGS)

CPP = $(filter-out main.cpp, $(filter-out test.cpp, $(filter-out $(TSTSRC), $(wildcard *.cpp))))
OBJ = $(CPP:.cpp=.o)

TSTSRC = $(wildcard *_test.cpp)
TSTOBJ = $(TSTSRC:.cpp=.o)

HDR = $(filter-out catch.hpp, $(wildcard *.h))

count: $(OBJ) main.o
	$(CC) $(LFLAGS) -o $@ $^

test: testdata $(OBJ) $(TSTOBJ) test.o
	$(CC) $(TESTFLAGS) -o $@ $(filter-out $<,$^)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: main.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

test.o: test.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean testdata

testdata: test_data/big.txt test_data/big_out_3.txt test_data/big_out_12.txt

test_data/big.txt: test_data/createSeqfile.py
	python test_data/createSeqfile.py test_data/big.txt 3000000

test_data/big_out_3.txt: test_data/big.txt test_data/computeKmers.py
	python test_data/computeKmers.py test_data/big.txt 3 test_data/big_out_3.txt

test_data/big_out_12.txt: test_data/big.txt test_data/computeKmers.py
	python test_data/computeKmers.py test_data/big.txt 12 test_data/big_out_12.txt

clean:
	rm -r *.o test count deps.mk test_data/big*.txt

depend:
	$(CC) $(CFLAGS) -MM $(CPP) $(TSTSRC) > deps.mk

-include deps.mk
