#include <iostream>
#include "std_dict.hh"
#include <string>


int main(){
	Std_Dictionary<int, std::string> f;
	f.set_key(1, "one");
	f.set_key(2, "two");
	f.set_key(3, "three");
	
	std::cout << f.get_value(1).value;
	std::cout << f.get_value(2).value;
	std::cout << f.get_value(3).value;
	std::cout << f.get_value(4).exists;
	

}
