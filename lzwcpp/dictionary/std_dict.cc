#include <iostream>
#include "std_dict.hh"

using namespace std;

template <typename key_type, typename value_type> Std_Dictionary<key_type, value_type>::Std_Dictionary (){
	map = new unordered_map<key_type, value_type>;
	end = map.cend();
}

template <typename key_type, typename value_type> Std_Dictionary<key_type, value_type>::~Std_Dictionary(){
	delete map;
}

template <typename key_type, typename value_type> void Std_Dictionary<key_type, value_type>::set_key(key_type key, value_type value){
	map[key] = value;
}

template <typename key_type, typename value_type> Dictionary_Entry<value_type> Std_Dictionary<key_type, value_type>::get_value(key_type key){

	auto lookup = map.find(key);
	if (lookup == end){
		return Dictionary_Entry(false);
	}
	return Dictionary_Entry(true, lookup->second());

}
