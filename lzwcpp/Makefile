CXX=g++
CXXFLAGS= -mbmi2 -Wall -Wextra -pedantic -std=c++2a -O3 -g -Werror
EXEC= test_lzw test_bitio lzwdecode lzwencode
BITIO_PATH=bitio
TEST_PATH=unittests

all: dictionary lzwencode lzwdecode test_lzw test_bitio

#dict
.PHONY: dictionary
dictionary: 
	$(MAKE) -C ./dictionary
# unittests

test_bitio: $(TEST_PATH)/test_bitio.o $(BITIO_PATH)/bitio.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

test_lzw: $(TEST_PATH)/test_lzw.o $(BITIO_PATH)/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#executables

lzwencode: lzwencode.o $(BITIO_PATH)/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: lzwdecode.o $(BITIO_PATH)/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
	$(MAKE) -C ./dictionary clean
