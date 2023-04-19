CXX=g++
CXXFLAGS= -mbmi2 -Wall -Wextra -pedantic -std=c++2a -O3 -Werror 
EXEC= test_lzw test_bitio lzwdecode lzwencode
BITIO_PATH=bitio
TEST_PATH=unittests
FOUR_TO_ONE_PATH=four_to_one

all: dictionary lzwencode lzwdecode test_lzw test_bitio four_to_one_encode four_to_one_decode

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

four_to_one_encode: $(FOUR_TO_ONE_PATH)/four_to_one_encode.o  $(FOUR_TO_ONE_PATH)/four_to_one.o $(BITIO_PATH)/bitio.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

four_to_one_decode: $(FOUR_TO_ONE_PATH)/four_to_one_decode.o $(FOUR_TO_ONE_PATH)/four_to_one.o $(BITIO_PATH)/bitio.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
	$(MAKE) -C ./dictionary clean
