
#include <iostream>
#include "std_dict.hh"
#include <string>
#include <cassert>
#include <sstream>

void test_std_dict(){
	Std_Encode_Dictionary<int> f;

	f.add_string("Cool", 1);
	std::cout << f.code_of("Cool", 4);

	assert(f.code_of("Cool", 4) == 1);
}

int main(){
	test_std_dict();
	}
