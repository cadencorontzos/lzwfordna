CXX=g++
CXXFLAGS=-std=c++2a -g -O0 -Werror -Wall
EXEC= test_lzw test_bitio lzwdecode lzwencode

all: lzwencode lzwdecode test_lzw test_bitio

# unittests

test_bitio: unittests/test_bitio.o bitio/bitio.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

test_lzw: unittests/test_lzw.o bitio/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#executables

lzwencode: lzwencode.o bitio/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: lzwdecode.o bitio/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC) -f