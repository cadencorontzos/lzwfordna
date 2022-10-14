
# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CXX=g++
CXX_FLAGS=-Wall -Wextra -pedantic -std=c++17 -O3#-Werror
TARGETS=test_bitio
 
# ****************************************************
# Targets needed to bring the executable up to date
 

test_bitio: test_bitio.o bitio.o
	$(CXX) $(CXX_FLAGS) -o main test_bitio.o bitio.o 

lzw: lzw.o bitio.o
	$(CXX) $(CXX_FLAGS) -o lzw lzw.o bitio.o 

%.o: %.cc %.hh
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC) -f