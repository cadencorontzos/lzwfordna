
# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++2a -O3#-Werror
 
# ****************************************************
# Targets needed to bring the executable up to date
 

test_bitio: test_bitio.o bitio.o
	$(CXX) $(CXXFLAGS) -o main test_bitio.o bitio.o 
all: lzwencode lzwdecode

lzwencode: lzwencode.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: lzwdecode.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^
# lzw: lzw.o bitio.o
# 	$(CXX) $(CXXFLAGS) -o lzw lzw.o bitio.o 

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC) -f