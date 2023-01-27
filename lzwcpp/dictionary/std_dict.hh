#include <iostream>
#include "dictionary.hh"
#include <unordered_map>

using namespace std;
template <typename key_type, typename value_type> class Std_Dictionary: private Dictionary <key_type, value_type>{
 public:

	unordered_map<key_type, value_type> map;
	unordered_map<key_type, value_type>::const_iterator end;
	Std_Dictionary (): Dictionary<key_type, value_type>(), end(map.cend()){}

	void set_key(key_type key, value_type value){
		map[key] = value;
	}

	Dictionary_Entry<value_type> get_value(key_type key){
		auto lookup = map.find(key);
		if (lookup == end) { 
			return Dictionary_Entry<value_type>(false);
		}
		return Dictionary_Entry<value_type>(true, lookup->second);

	}
};

template <typename codeword_type> class Std_Encode_Dictionary: public LZW_Encode_Dictionary <Std_Dictionary<std::string, codeword_type>, codeword_type>{
 public:

	 Std_Encode_Dictionary (): LZW_Encode_Dictionary<Std_Dictionary<std::string, codeword_type>, codeword_type>(Std_Dictionary<std::string, codeword_type>()){};
};


template <typename codeword_type> class Std_Decode_Dictionary: public LZW_Decode_Dictionary <Std_Dictionary<codeword_type, std::string>, codeword_type>{
 public:

	Std_Decode_Dictionary (): LZW_Decode_Dictionary<Std_Dictionary<codeword_type, std::string>, codeword_type>(Std_Dictionary<codeword_type, std::string>()){};
};
