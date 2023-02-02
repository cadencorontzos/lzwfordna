
#include <iostream>
#include "std_dict.hh"
#include <string>
#include <cassert>
#include <sstream>

void test_code_of(){
	
	Std_Encode_Dictionary<int> f;

	f.add_string("one", 1);
	f.add_string("two", 2);
	f.add_string("three", 3);


	assert(f.code_of("one", 3) == 1);
	assert(f.code_of("two", 3) == 2);
	assert(f.code_of("three", 5) == 3);


}

void test_find_longest_in_dict(){
	Std_Encode_Dictionary<int> f;

	f.add_string("o", 1);
	f.add_string("ot", 2);
	f.add_string("ot3", 3);


	std::string input = "ot3f";
	std::stringstream input_file(input);


	Std_Encode_Dictionary<int>::Std_Dict_Entry ret = f.find_longest_in_dict(input_file);
	
	assert(ret.str == "ot3");
	assert(ret.codeword = 3);


}
void test_std_dict(){
	Std_Encode_Dictionary<int> f;

	f.add_string("Cool", 1);
	std::cout << f.code_of("Cool", 4);

	assert(f.code_of("Cool", 4) == 1);
}

void test_std_decode_dict(){
	Std_Decode_Dictionary<int> f;

	f.add_string("Cool", 1);


	assert(f.str_of(1) == "Cool");

}

int main(){
	test_std_dict();
	test_code_of();
	test_find_longest_in_dict();
	test_std_decode_dict();
	}
