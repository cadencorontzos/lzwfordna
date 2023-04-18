CXX=g++
CXXFLAGS= -mbmi2 -Wall -Wextra -pedantic -std=c++2a -O3 -Werror 
EXEC= test_lzw test_bitio lzwdecode lzwencode
BITIO_PATH=bitio
TEST_PATH=unittests
ENTROPY_PATH=entropy-encoding

all: dictionary turborc lzwencode lzwdecode test_lzw test_bitio

#dic
.PHONY: dictionary
dictionary: 
	$(MAKE) -C ./dictionary
#entropy
.PHONY: turborc
turborc: 
	$(MAKE) -C ./$(ENTROPY_PATH)
# unittests

test_bitio: $(TEST_PATH)/test_bitio.o $(BITIO_PATH)/bitio.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

test_lzw: $(TEST_PATH)/test_lzw.o $(BITIO_PATH)/bitio.o lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#executables

lzwencode: lzwencode.o $(BITIO_PATH)/bitio.o lzw.o $(ENTROPY_PATH)/turborc.o $(ENTROPY_PATH)/libsais/src/libsais.o $(ENTROPY_PATH)/libsais/src/libsais16.o $(ENTROPY_PATH)/librc.a
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: lzwdecode.o $(BITIO_PATH)/bitio.o lzw.o $(ENTROPY_PATH)/turborc.o $(ENTROPY_PATH)/libsais/src/libsais.o $(ENTROPY_PATH)/libsais/src/libsais16.o $(ENTROPY_PATH)/librc.a
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwencode.o: lzwencode.cc
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c -o $@ $<

lzwdecode.o: lzwdecode.cc
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c -o $@ $<
%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
	$(MAKE) -C ./dictionary clean
	$(MAKE) -C ./$(ENTROPY_PATH) clean
