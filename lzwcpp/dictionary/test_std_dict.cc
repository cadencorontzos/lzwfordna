#include <iostream>
#include "std_dict.hh"
#include <string>
#include <cassert>
#include <sstream>

void test_std_dict(){
	Std_Decode_Dictionary<int> f;
	f.dictionary.set_key(1, "one");
	f.dictionary.set_key(2, "two");
	f.dictionary.set_key(3, "three");
	
	assert( f.dictionary.get_value(1).value == "one");
	assert( f.dictionary.get_value(2).value == "two");
	assert( f.dictionary.get_value(3).value == "three");
	assert( !f.dictionary.get_value(4).exists);
}
void test_get_longest_in_dict(){

	Std_Encode_Dictionary<int> f;
	f.dictionary.set_key("o", 1);
	f.dictionary.set_key("ot", 2);
	f.dictionary.set_key("ot3", 3);

	std::string input = "ot3f"	;
	std::stringstream input_file(input);

	LZW_Dictionary_Entry<int> ret =  f.get_longest_in_dict(input_file);

	assert(ret.string_value == "ot3");
	assert(ret.codeword_value== 3);
}

int main(){
	test_std_dict();
	test_get_longest_in_dict();
	}
