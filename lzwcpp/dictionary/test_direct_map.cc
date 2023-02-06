

#include <iostream>
#include "direct_mapped_dict.hh"
#include <string>
#include <cassert>
#include <sstream>

void test_code_of(){
	
	Direct_Mapped_Dictionary<> f;

	f.add_string("A", 1);
	f.add_string("AT", 2);
	f.add_string("ACT", 3);


	assert(f.code_of("A", 1) == 1);
	assert(f.code_of("AT", 2) == 2);
	assert(f.code_of("ACT", 3) == 3);


}


void test_find_longest_in_dict(){
	Direct_Mapped_Dictionary<> f;

	f.add_string("A", 1);
	f.add_string("AC", 2);
	f.add_string("ACT", 3);


	std::string input = "ACTG";
	std::stringstream input_file(input);


	Direct_Mapped_Dictionary<>::Dict_Entry ret ;
	ret = f.find_longest_in_dict(input_file);
	
	assert(ret.str == "ACT");
	assert(ret.codeword = 3);
	char c = 'G';
	assert(char(input_file.get()) == c); 

}
void test_direct_dict(){
	Direct_Mapped_Dictionary<> f;

	f.add_string("ACTGC", 1);

	assert(f.code_of("ACTGC", 5) == 1);
}


int main(){
	test_direct_dict();
	test_code_of();
	test_find_longest_in_dict();
}
