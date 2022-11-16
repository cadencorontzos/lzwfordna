#include "../lzw.hh"
#include "../bitio/bitio.hh"

#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>

// empty
std::string empty_decoded = "";
// | eof      | 0| trailing
// |10000000 0|00|00000
char empty_encoded[] = {char(128), char(0)};
int empty_encoded_length = 2;

// 01100110
std::string single_char_decoded = "f";
// eof       | 1| f       | trailing
// 10000000 0|01|01100 110|00000
char single_char_encoded[] = { char(128), char(44), char(192)};
int single_char_encoded_length = 3;

// 01100001 01100010
std::string two_chars_decoded = "ab";
// | cw for a | b       | eof      | 0| trailing
// |00110000 1|0110001 0|1000000 00|00|0000
char two_chars_encoded[] = { char(48), char(177), char(64), char(0)};
int two_chars_encoded_length = 4;

//01100001 01100010 01100110
std::string three_chars_decoded = "abab";
// | cw for a | b       | eof      | 3| cw for ab| trailing
// |00110000 1|0110001 0|1000000 00|11|1000 00001|000
char three_chars_encoded[] = { char(48), char(177), char(64), char(56), char(8)};
int three_chars_encoded_length = 5;


////////////////////////////////////////////////////////////
// encode unit tests 

void test_encode(std::string decoded_string, char* encoded_chars, int encoded_chars_length){
    std::stringstream input(decoded_string);
    std::stringstream output;

    {
        LZW lzw;
        lzw.encode(input, output);
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
    std::stringstream input(encoded_string);
    std::stringstream output;

    {
        LZW lzw;
        lzw.decode(input, output);
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