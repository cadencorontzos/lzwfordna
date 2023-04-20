CXX=g++
CXXFLAGS= -mbmi2 -Wall -Wextra -pedantic -std=c++2a -O3  -Werror 
EXEC= test_lzw test_bitio lzwdecode lzwencode
BITIO_PATH=bitio
TEST_PATH=unittests
ENTROPY_PATH=entropy-encoding
THREE_STREAM_PATH=three_stream_lzw

all: dictionary turborc three_stream_lzwencode three_stream_lzwdecode test_three_stream_lzw test_bitio

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

test_three_stream_lzw: $(THREE_STREAM_PATH)/test_three_stream_lzw.o $(BITIO_PATH)/bitio.o $(THREE_STREAM_PATH)/three_stream_lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#executables

three_stream_lzwencode: three_stream_lzwencode.o $(BITIO_PATH)/bitio.o three_stream_lzw.o $(ENTROPY_PATH)/turborc.o $(ENTROPY_PATH)/libsais/src/libsais.o $(ENTROPY_PATH)/libsais/src/libsais16.o $(ENTROPY_PATH)/librc.a
	$(CXX) $(CXXFLAGS) -o $@ $^

three_stream_lzwdecode: three_stream_lzwdecode.o $(BITIO_PATH)/bitio.o three_stream_lzw.o $(ENTROPY_PATH)/turborc.o $(ENTROPY_PATH)/libsais/src/libsais.o $(ENTROPY_PATH)/libsais/src/libsais16.o $(ENTROPY_PATH)/librc.a
	$(CXX) $(CXXFLAGS) -o $@ $^

three_stream_lzwencode.o: three_stream_lzwencode.cc
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c -o $@ $<

three_stream_lzwdecode.o: three_stream_lzwdecode.cc
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c -o $@ $<
%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
	$(MAKE) -C ./dictionary clean
	$(MAKE) -C ./$(ENTROPY_PATH) clean
