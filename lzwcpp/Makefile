CXX=g++
CXXFLAGS=-std=c++2a -pg -g
EXEC= test_lzw test_bitio lzwdecode lzwencode

all: lzwencode lzwdecode test_lzw test_bitio

test_bitio: test_bitio.o bitio.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

test_lzw: test_lzw.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwencode: lzwencode.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: lzwdecode.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC) -f