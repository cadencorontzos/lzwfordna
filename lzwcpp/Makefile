CXX=g++
CXXFLAGS= -mbmi2 -Wall -Wextra -pedantic -std=c++2a -O3  -Werror 
EXEC= test_lzw test_bitio lzwdecode lzwencode three_stream_lzwencode three_stream_lzwdecode test_three_stream_lzw four_to_one_encode four_to_one_decode

BITIO_PATH=bitio
TEST_PATH=unittests
ENTROPY_PATH=entropy-encoding
THREE_STREAM_PATH=three_stream_lzw
FOUR_TO_ONE_PATH=four_to_one
LZW_PATH=lzw

all: dictionary turborc lzwencode lzwdecode three_stream_lzwencode three_stream_lzwdecode test_three_stream_lzw test_bitio test_lzw four_to_one_encode four_to_one_decode
test: dictionary test_lzw test_three_stream_lzw test_bitio

#dic
.PHONY: dictionary
dictionary: 
	$(MAKE) -C ./dictionary

#entropy
.PHONY: turborc
turborc: 
	$(MAKE) -C ./$(ENTROPY_PATH)

# unittests

test_bitio: $(BITIO_PATH)/test_bitio.o $(BITIO_PATH)/bitio.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

test_three_stream_lzw: $(THREE_STREAM_PATH)/test_three_stream_lzw.o $(BITIO_PATH)/bitio.o $(THREE_STREAM_PATH)/three_stream_lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test_lzw: $(LZW_PATH)/test_lzw.o $(BITIO_PATH)/bitio.o $(LZW_PATH)/lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

#executables

# lzw
lzwencode: $(LZW_PATH)/lzwencode.o $(BITIO_PATH)/bitio.o $(LZW_PATH)/lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

lzwdecode: $(LZW_PATH)/lzwdecode.o $(BITIO_PATH)/bitio.o $(LZW_PATH)/lzw.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# three stream lzw
three_stream_lzwencode: $(THREE_STREAM_PATH)/three_stream_lzwencode.o $(BITIO_PATH)/bitio.o $(THREE_STREAM_PATH)/three_stream_lzw.o $(ENTROPY_PATH)/turborc.o $(ENTROPY_PATH)/libsais/src/libsais.o $(ENTROPY_PATH)/libsais/src/libsais16.o $(ENTROPY_PATH)/librc.a
	$(CXX) $(CXXFLAGS) -o $@ $^

three_stream_lzwdecode: $(THREE_STREAM_PATH)/three_stream_lzwdecode.o $(BITIO_PATH)/bitio.o $(THREE_STREAM_PATH)/three_stream_lzw.o $(ENTROPY_PATH)/turborc.o $(ENTROPY_PATH)/libsais/src/libsais.o $(ENTROPY_PATH)/libsais/src/libsais16.o $(ENTROPY_PATH)/librc.a
	$(CXX) $(CXXFLAGS) -o $@ $^

$(THREE_STREAM_PATH)/three_stream_lzwencode.o: $(THREE_STREAM_PATH)/three_stream_lzwencode.cc
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c -o $@ $<

 $(THREE_STREAM_PATH)/three_stream_lzwdecode.o: $(THREE_STREAM_PATH)/three_stream_lzwdecode.cc
	$(CXX) $(CXXFLAGS) -Wno-write-strings -c -o $@ $<

# four to one
four_to_one_encode: $(FOUR_TO_ONE_PATH)/four_to_one_encode.o  $(FOUR_TO_ONE_PATH)/four_to_one.o $(BITIO_PATH)/bitio.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

four_to_one_decode: $(FOUR_TO_ONE_PATH)/four_to_one_decode.o $(FOUR_TO_ONE_PATH)/four_to_one.o $(BITIO_PATH)/bitio.o 
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm  *.o */*.o $(EXEC) -f
	$(MAKE) -C ./dictionary clean
	$(MAKE) -C ./$(ENTROPY_PATH) clean
