#include "../lzw.hh"
#include "../bitio/bitio.hh"

#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>

// empty
std::string empty_decoded = "";
// | eof              | 0| trailing
// |00000000 00000101 |00|000000
char empty_encoded[] = {char(0), char(5), char(0)};
int empty_encoded_length = 3;

// 01100110
std::string single_char_decoded = "A";
// eof       		 | 1| A       | trailing
// 00000000 00000101 |01|010000 01|000000
char single_char_encoded[] = { char(0), char(5), char(80), char(64) };
int single_char_encoded_length = 4;

// 01100001 01100010
std::string two_chars_decoded = "AG";
// | cw for A         | G       | eof              | 0| trailing 
// |00000000 00000001 |01000111 |00000000  00000101 00|000000
char two_chars_encoded[] = { char(0), char(1), char(71), char(0), char(5), char(0)};
int two_chars_encoded_length = 6;

//01100001 01100010 01100110
std::string three_chars_decoded = "AGAG";
// | cw for A         | G       | eof              | 0| cw for AG        | trailing
// |00000000 00000001 |01000111 |00000000  00000101 11|000000 00000001 10|000000
char three_chars_encoded[] = { char(0), char(1), char(71), char(0), char(5), char(192), char(1), char(128) };
int three_chars_encoded_length = 8;


////////////////////////////////////////////////////////////
// encode unit tests 

void test_encode(std::string decoded_string, char* encoded_chars, int encoded_chars_length){
	 std::cout << "Testing string " << decoded_string << std::endl; // for debug
    std::stringstream output;
	const char* input_file = decoded_string.c_str();
	int file_size = decoded_string.length();
    {
        LZW lzw;
        lzw.encode(input_file, file_size, output);
    }
    
    std::string expected_output(encoded_chars, encoded_chars_length);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void encode_tests(){
    test_encode(empty_decoded, empty_encoded, empty_encoded_length);
    test_encode(single_char_decoded, single_char_encoded, single_char_encoded_length);
    test_encode(two_chars_decoded, two_chars_encoded, two_chars_encoded_length);
    test_encode(three_chars_decoded, three_chars_encoded, three_chars_encoded_length);
}

////////////////////////////////////////////////////////////
// decode unit tests

void test_decode(char* encoded_chars, int encoded_length, std::string decoded_string){
    std::string encoded_string(encoded_chars, encoded_length);
    std::stringstream output;

    {
        LZW lzw;
        lzw.decode(encoded_chars, output);
    }
    
    std::string expected_output(decoded_string);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void decode_tests(){
    test_decode(empty_encoded, empty_encoded_length, empty_decoded);
    test_decode(single_char_encoded, single_char_encoded_length, single_char_decoded);
    test_decode(two_chars_encoded, two_chars_encoded_length, two_chars_decoded);
    test_decode(three_chars_encoded, three_chars_encoded_length, three_chars_decoded);
}

////////////////////////////////////////////////////////////

int main(){
    encode_tests();
    decode_tests();
}
