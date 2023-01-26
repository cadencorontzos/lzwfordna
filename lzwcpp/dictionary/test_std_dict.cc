#include <iostream>
#include "std_dict_two.hh"
#include <string>


int main(){
	Std_Decode_Dictionary<int> f;
	f.dictionary.set_key(1, "one");
	f.dictionary.set_key(2, "two");
	f.dictionary.set_key(3, "three");
	
	std::cout << f.dictionary.get_value(1).value;
	std::cout << f.dictionary.get_value(2).value;
	std::cout << f.dictionary.get_value(3).value;
	std::cout << f.dictionary.get_value(4).exists;
	

}
