#include "../lzw.hh"
#include "../dictionary/std_dict.hh"
#include "../bitio/bitio.hh"
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>

std::string to_bits(std::string str){
	std::string res = "";
	
	for(unsigned index = 0; index < str.length(); index++){

		for(int i = CHAR_BIT-1; i>=0; i--){
			
        	res += std::to_string(1&str[index]<<i);
		}

	}
	return res;
}

void detailed_assert(std::string expected, std::string output){
	if(expected!=output){
		std::cout << "Test Failed: " << std::endl;
		std::cout << "Expected: " << std::endl;
		std::cout << "    literal string: " << expected << std::endl;
		std::cout << "    bits " << to_bits(expected) << std::endl;
		std::cout << "Found: " << std::endl;
		std::cout << "    literal string: " << expected << std::endl;
		std::cout << "    bits " << to_bits(output) << std::endl;

	}
	assert(expected==output);
}

// empty
std::string empty_decoded = "";
// we expect
// | eof              | 0| trailing
int empty_encoded_length;
std::string empty_expected_output(){
	Codeword_Helper codeword_helper;
	std::stringstream bits;
	{
		BitOutput bito(bits);
		// eof
		bito.output_n_bits(codeword_helper.EOF_CODEWORD, codeword_helper.bits_per_codeword);
		// 00
		bito.output_bit(0);
		bito.output_bit(0);

	}// trailing
	const std::string& tmp = bits.str();   
	return tmp;
}

// 01100110
std::string single_char_decoded = "A";
// we expect
// eof       		 | 1| A       | trailing
std::string single_char_expected_output(){

	Codeword_Helper codeword_helper;
	std::stringstream bits;
	{
		BitOutput bito(bits);
		// eof
		bito.output_n_bits(codeword_helper.EOF_CODEWORD, codeword_helper.bits_per_codeword);
		// 01
		bito.output_bit(0);
		bito.output_bit(1);

		//A
		bito.output_n_bits('A', CHAR_BIT);

	}// trailing
	const std::string& tmp = bits.str();   

	return tmp;
}

// 01100001 01100010
std::string two_chars_decoded = "AG";
// | cw for A         | G       | eof              | 0| trailing 
std::string two_chars_expected_output(){
	
	Codeword_Helper codeword_helper;
	std::stringstream bits;
	{
		BitOutput bito(bits);
		// cw for A 
		bito.output_n_bits(1, codeword_helper.bits_per_codeword);
		// G
		bito.output_n_bits('G', CHAR_BIT);
		// eof
		bito.output_n_bits(codeword_helper.EOF_CODEWORD, codeword_helper.bits_per_codeword);

		//00
		bito.output_bit(0);
		bito.output_bit(0);


	}// trailing
	const std::string& tmp = bits.str();   

	return tmp;
}

//01100001 01100010 01100110
std::string three_chars_decoded = "AGAG";
// | cw for A         | G       | eof              | 11| cw for AG        | trailing
std::string three_chars_expected_output(){

	Codeword_Helper codeword_helper;
	std::stringstream bits;
	{
		BitOutput bito(bits);
		// cw for A 
		bito.output_n_bits(1, codeword_helper.bits_per_codeword);
		// G
		bito.output_n_bits('G', CHAR_BIT);
		// eof
		bito.output_n_bits(codeword_helper.EOF_CODEWORD, codeword_helper.bits_per_codeword);

		//11
		bito.output_bit(1);
		bito.output_bit(1);

		// cw for AG
		bito.output_n_bits(codeword_helper.get_next_codeword(), codeword_helper.bits_per_codeword);


	}// trailing
	const std::string& tmp = bits.str();   

	return tmp;
}


////////////////////////////////////////////////////////////
// encode unit tests 

void test_encode(std::string decoded_string, std::string expected_output){
	 std::cout << "Testing string :" << decoded_string << std::endl; // for debug
    std::stringstream output;
	const char* input_file = decoded_string.c_str();
	int file_size = decoded_string.length();
    {
        LZW lzw;
        lzw.encode(input_file, file_size, output);
    }
    
    std::string output_string = output.str();
    detailed_assert( expected_output, output_string);
}

void encode_tests(){
    test_encode(empty_decoded, empty_expected_output());
	test_encode(single_char_decoded, single_char_expected_output());
    test_encode(two_chars_decoded, two_chars_expected_output());
    test_encode(three_chars_decoded, three_chars_expected_output());
}

////////////////////////////////////////////////////////////
// decode unit tests

void test_decode(std::string encoded_string, std::string decoded_string){
    std::stringstream output;

    {
        LZW lzw;
        lzw.decode(encoded_string.c_str(), output);
    }
    
    std::string expected_output(decoded_string);
    std::string output_string = output.str();
    detailed_assert(expected_output, output_string);
}

void decode_tests(){
    test_decode(empty_expected_output(), empty_decoded);
    test_decode(single_char_expected_output(), single_char_decoded);
    test_decode(two_chars_expected_output(), two_chars_decoded);
    test_decode(three_chars_expected_output(), three_chars_decoded);
}

////////////////////////////////////////////////////////////

int main(){
	std::cout << "Testing LZW for max codeword size of  " << CODEWORD_SIZE<< std::endl;
    encode_tests();
    decode_tests();
}
