#include <iostream>
#include "dictionary.hh"
#include <unordered_map>

using namespace std;
template <typename key_type, typename value_type> class Std_Dictionary: private Dictionary <key_type, value_type>{
 public:

	unordered_map<key_type, value_type> map;
	Std_Dictionary (): Dictionary<key_type, value_type>(){}

	void set_key(key_type key, value_type value){
		map[key] = value;
	}

	Dictionary_Entry<value_type> get_value(key_type key){
		auto lookup = map.find(key);
		if (lookup == map.cend()){
			return Dictionary_Entry<value_type>(false);
		}
		return Dictionary_Entry<value_type>(true, lookup->second);

	}
};
