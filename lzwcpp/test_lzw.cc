#include "lzw.hh"
#include "bitio.hh"

#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>

// empty
std::string empty_decoded = "";
// |10000000 0|0000000
char empty_encoded[] = {char(128), char(0)};
int empty_encoded_length = 2;

// 01100110
std::string single_char_decoded = "f";
// |10000000 1|0110011 0|1000000 00|000000 
char single_char_encoded[] = { char(128), char(179), char(64), char(0)};
int single_char_encoded_length = 4;

// 01100001 01100010
std::string two_chars_decoded = "ab";
// |00110000 1|0110001 0|1000000 00|000000
char two_chars_encoded[] = { char(48), char(177), char(64), char(0)};
int two_chars_encoded_length = 4;


////////////////////////////////////////////////////////////
// encode unit tests 

void test_encode_empty(){
    std::stringstream input(empty_decoded);
    std::stringstream output;

    {
        LZW lzw;
        lzw.encode(input, output);
    }
    
    std::string expected_output(empty_encoded, empty_encoded_length);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void test_encode_single_char(){

    std::stringstream input(single_char_decoded);
    std::stringstream output;

    {
        LZW lzw;
        lzw.encode(input, output);
    }
    
    std::string expected_output(single_char_encoded, single_char_encoded_length);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void test_encode_two_chars(){
    std::stringstream input(two_chars_decoded);
    std::stringstream output;

    {
        LZW lzw;
        lzw.encode(input, output);
    }
    
    std::string expected_output(two_chars_encoded, two_chars_encoded_length);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void test_encode(){
    test_encode_empty();
    test_encode_single_char();
    test_encode_two_chars();
}



////////////////////////////////////////////////////////////
// decode unit tests

void test_decode_empty(){
    std::string encoded_string(empty_encoded, empty_encoded_length);
    std::stringstream input(encoded_string);
    std::stringstream output;

    {
        LZW lzw;
        lzw.decode(input, output);
    }
    
    std::string expected_output(empty_decoded);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void test_decode_single_char(){

    std::string encoded_string(single_char_encoded, single_char_encoded_length);
    std::stringstream input(encoded_string);
    std::stringstream output;

    {
        LZW lzw;
        lzw.decode(input, output);
    }
    
    std::string expected_output(single_char_decoded);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void test_decode_two_chars(){
    std::string encoded_string(two_chars_encoded, two_chars_encoded_length);
    std::stringstream input(encoded_string);
    std::stringstream output;

    {
        LZW lzw;
        lzw.decode(input, output);
    }
    
    std::string expected_output(two_chars_decoded);
    std::string output_string = output.str();
    assert(output_string == expected_output);
}

void test_decode(){
    test_decode_empty();
    test_decode_single_char();
    test_decode_two_chars();
}

int main(){
    test_encode();
    test_decode();
}