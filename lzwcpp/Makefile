
# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CXX=g++
CXX_FLAGS=-g -std=c++14 -Wall -g
TARGETS=test_bitio
 
# ****************************************************
# Targets needed to bring the executable up to date
 

test_bitio: test_bitio.o bitio.o
	$(CXX) $(CXX_FLAGS) -o main test_bitio.o bitio.o 

lzw: lzw.o bitio.o
	$(CXX) $(CXX_FLAGS) -o lzw lzw.o bitio.o 

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC) -f