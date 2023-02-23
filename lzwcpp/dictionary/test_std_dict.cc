
#include <iostream>
#include "std_dict.hh"
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
void test_add_string_encode(){
	LZW_Encode_Dictionary f;

	f.add_string("ACTGC", 5, 1);

	assert(f.code_of("ACTGC", 5) == 1);
}

void test_encode(){
	test_add_string_encode();
	test_code_of();
	test_find_longest_in_dict();
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
	for(unsigned i = codeword_helper.current_codeword; i < MAX_CODEWORD; i++){

		assert(i == codeword_helper.get_next_codeword());
		// make sure bits per codeword is just right
		bool less_than_upper = (i <  static_cast<unsigned>(1<< codeword_helper.bits_per_codeword)|| (codeword_helper.bits_per_codeword ==32 && i >= static_cast<unsigned>(1<<31)));
		bool greater_than_lower = (i >= static_cast<unsigned>(1<<( codeword_helper.bits_per_codeword-1)));
		assert(less_than_upper && greater_than_lower) ;
	}
	codeword_helper.get_next_codeword();
	// make sure we didn't go over max
	assert(codeword_helper.get_next_codeword() == MAX_CODEWORD);
}

int main(){
	test_encode();
	test_decode();
	test_codeword_helper();
	return 0;
}
