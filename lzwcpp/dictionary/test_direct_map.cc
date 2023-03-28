#include <iostream>
#include "direct_mapped_dict.hh"
#include <string>
#include <cassert>

// tests for endcode dictionary
//
//

void test_code_of(){

	LZW_Encode_Dictionary f;
	
	// load strings into dictionary
	f.add_string("A", 1, 1);
	f.add_string("AT", 2, 2);
	f.add_string("ACT", 3, 3);

	// make sure codes of those strings are right
	assert(f.code_of("A", 1) == 1);
	assert(f.code_of("AT", 2) == 2);
	assert(f.code_of("ACT", 3) == 3);
}


void test_find_longest_in_dict(){

	// test string shorter than max string length

	{

		LZW_Encode_Dictionary f;

		// load three strings, each building on last
		f.add_string("A", 1, 1);
		f.add_string("AC", 2, 2);
		f.add_string("ACT", 3, 3);

		// for this input, "ACT" should be the longest run in the dict
		std::string input = "ACTG";

		int ret;
		ret = f.find_longest_in_dict(input.c_str(), input.c_str()+ input.length());
		std::string output = input.substr(0,ret);
		assert(output == "ACT");
		assert(f.code_of(output.c_str(), 3) == 3);
	}


	{

		LZW_Encode_Dictionary f;

		// load three strings, each building on last
		f.add_string("A", 1, 1);
		f.add_string("AC", 2, 2);
		f.add_string("ACT", 3, 3);

		// for this input, "ACT" should be the longest run in the dict
		std::string input = "ACTGACTGACTGACTGACTGACTG";

		int ret;
		ret = f.find_longest_in_dict(input.c_str(), input.c_str()+ input.length());
		std::string output = input.substr(0,ret);
		assert(output == "ACT");
		assert(f.code_of(output.c_str(), 3) == 3);
	}

	// test string longer than max string length
	// We assume the max is shorter than 20 here

	// we are not letting in strings longer than max length so we don't need this 
	/* { */
	/* 	LZW_Encode_Dictionary f; */


	/* 	// for this input, "ACT" should be the longest run in the dict */
	/* 	std::string input = "ACTGACTGACTGACTGACTGACTG"; */
		
	/* 	// load all substrings into the the dictionary */
	/* 	for(unsigned i = 1; i < input.length()-1; i++){ */
	/* 		f.add_string(input.substr(0,i).c_str(), i, i); */
	/* 	} */
	/* 	int ret; */
	/* 	ret = f.find_longest_in_dict(input.c_str(), input.c_str()+ input.length()); */
	/* 	std::string output = input.substr(0,ret); */
	
	/* 	assert(output == input.substr(0, input.length()-2)); */
	/* 	assert(f.code_of(output.c_str(), input.length()-2) == input.length()-2); */
	/* } */


	// test string equal to max string length 

	{
		LZW_Encode_Dictionary f;

		// build string
		char oneA = 'A';
		std::string input = "";
		for(unsigned i = 1; i <= MAX_STRING_LENGTH; i++){
			input+=oneA;
			f.add_string(input.c_str(), i, i);
		}
		
		int ret;
		ret = f.find_longest_in_dict(input.c_str(), input.c_str()+ input.length());
		std::string output = input.substr(0,ret);
	
		assert(output == input);
		assert(f.code_of(output.c_str(), input.length()) == MAX_STRING_LENGTH);
	}

}


void test_find_longest_looping_down(){

	LZW_Encode_Dictionary f;

	// load three strings, each building on last
	f.add_string("A", 1, 1);
	f.add_string("AC", 2, 2);

	// for this input, "ACT" should be the longest run in the dict
	std::string input = "ACTG";
	int index = (0<<6) + (1 << 4) + (2<<2) + 3;
	int ret;
	ret = f.find_longest_looping_down(4, index);
	std::string output = input.substr(0,ret);
	assert(output == "AC");
	assert(f.code_of(output.c_str(), 2) == 2);

}

void test_find_longest_looping_up(){

	LZW_Encode_Dictionary f;

	// load three strings, each building on last
	f.add_string("A", 1, 1);
	f.add_string("AC", 2, 2);
	f.add_string("ACT", 3, 3);

	// for this input, "ACT" should be the longest run in the dict
	std::string input = "ACTG";

	int ret;
	ret = f.find_longest_looping_up_on_fly(input.c_str(), input.c_str()+ input.length(), 0,0);
	std::string output = input.substr(0,ret);
	assert(output == "ACT");
	assert(f.code_of(output.c_str(), 3) == 3);

}

void test_find_longest_binary_search(){

	// test len < pivot
	{
		LZW_Encode_Dictionary f;

		// load three strings, each building on last
		f.add_string("A", 1, 1);
		f.add_string("AC", 2, 2);
		f.add_string("ACT", 3, 3);

		// for this input, "ACT" should be the longest run in the dict
		std::string input = "ACTGACTGACTGACTGACTG";

		int ret;
		ret = f.find_longest_binary_search(input.c_str(), 1, MAX_STRING_LENGTH);
		std::string output = input.substr(0,ret);
		assert(output == "ACT");
		assert(f.code_of(output.c_str(), 3) == 3);
	}

	// test len > pivot
	// assumes MAX_CODEWORD is between 10 and 15
	/* { */

	/* 	LZW_Encode_Dictionary f; */

	/* 	// for this input, "ACT" should be the longest run in the dict */
	/* 	std::string input = "ACTGACTGACTGACTGACTG"; */

	/* 	for(unsigned i = 1; i < 10; i++){ */
	/* 		f.add_string(input.substr(0,i).c_str(), i, i); */
	/* 	} */

	/* 	int ret; */
	/* 	ret = f.find_longest_binary_search(input.c_str()); */
	/* 	std::string output = input.substr(0,ret); */
	/* 	assert(output == input.substr(0, 9)); */
	/* 	assert(f.code_of(output.c_str(), 9) == 9); */
	/* } */

	// test len = pivot
	{
		LZW_Encode_Dictionary f;

		// for this input, "ACT" should be the longest run in the dict
		std::string input = "ACTGACTGACTGACTGACTG";
		unsigned pivot = std::floor(7/2);
		for(unsigned i = 1; i <= pivot; i++){
			f.add_string(input.substr(0,i).c_str(), i, i);
		}
		int ret;
		ret = f.find_longest_binary_search(input.c_str(), 1, MAX_STRING_LENGTH);
		std::string output = input.substr(0,ret);
		assert(output == input.substr(0,pivot));
		assert(f.code_of(output.c_str(), pivot) == pivot);
	}
}

void test_add_string_encode(){
	LZW_Encode_Dictionary f;

	f.add_string("ACTGC", 5, 1);

	assert(f.code_of("ACTGC", 5) == 1);
}

void test_encode(){
	test_add_string_encode();
	test_code_of();
	test_find_longest_in_dict();
	test_find_longest_looping_down();
	test_find_longest_looping_up();
	test_find_longest_binary_search();
}

// tests for decode dictionary
//
//

void test_add_string_decode(){
	LZW_Decode_Dictionary f;
	f.add_string("ACTGC", 1);
	assert(f.str_of(1) == "ACTGC");
}

void test_str_of(){

	LZW_Decode_Dictionary f;

	// add strings into decode dict
	f.add_string("A", 1);
	f.add_string("AT", 2);
	f.add_string("ACT", 3);

	// make sure lookups are correct
	assert(f.str_of(1) == "A");
	assert(f.str_of(2) == "AT");
	assert(f.str_of(3) == "ACT");
}

void test_decode(){
	test_add_string_decode();
	test_str_of();
}


// test for codeword_helper
//
//

void test_codeword_helper(){

	Codeword_Helper codeword_helper;

	// check that it is set up correctly
	assert(codeword_helper.EOF_CODEWORD == 5);
	assert(codeword_helper.current_codeword == 6);

	// check that helper increments correctly
	for(int i = codeword_helper.current_codeword; i < (1<<codeword_helper.bits_per_codeword); i++){
		assert(i == codeword_helper.get_next_codeword());
	}
	// make sure we didn't go over max
	assert(codeword_helper.get_next_codeword() == (1<<codeword_helper.bits_per_codeword) -1);
}

int main(){
	test_encode();
	test_decode();
	test_codeword_helper();
	return 0;
}
