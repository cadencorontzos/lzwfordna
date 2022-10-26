CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++2a -O3 -Werror
 
all: lzwencode lzwdecode

test_bitio: test_bitio.o bitio.o
	$(CXX) $(CXXFLAGS) -o main test_bitio.o bitio.o 

lzwencode: lzwencode.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: lzwdecode.o bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC) -f