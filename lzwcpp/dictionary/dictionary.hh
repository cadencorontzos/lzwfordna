#include <iostream>

template <typename value_type> struct Dictionary_Entry{
	public:
		value_type value;
		bool exists;
		Dictionary_Entry(bool exist, value_type val) : value(val), exists(exist){};
		Dictionary_Entry(bool exist) : exists(exist){};
		Dictionary_Entry(){};
};

template <typename key_type, typename value_type> class Dictionary {
 public:
	Dictionary() = default;
	virtual ~Dictionary() = default;
	virtual void set_key(key_type key, value_type value) = 0;
	virtual	Dictionary_Entry<value_type> get_value(key_type key)= 0;
};

template <typename codeword_type> struct LZW_Dictionary_Entry {
	std::string string_value;
	codeword_type codeword_value;

};

template <typename dict_type, typename codeword_type> class LZW_Encode_Dictionary {
	public:
		dict_type dictionary;
		LZW_Encode_Dictionary( dict_type dict): dictionary(dict){};
		LZW_Dictionary_Entry<codeword_type> get_longest_in_dict(std::istream& input){
			char next_character = input.get();
			std::string current_string_seen = "";
			std::string string_seen_plus_new_char;
			Dictionary_Entry<codeword_type> seen_previously(false, 0);
			while( next_character != EOF){
				
				string_seen_plus_new_char = current_string_seen + next_character;
				Dictionary_Entry<codeword_type> entry;
			   	entry	= dictionary.get_value(string_seen_plus_new_char);
				if (entry.exists){
					current_string_seen = string_seen_plus_new_char;
					seen_previously = entry;
				}else{
					LZW_Dictionary_Entry<codeword_type> longest{ current_string_seen, seen_previously.value};
					return longest; 

				}
				next_character = input.get();

			}	
			LZW_Dictionary_Entry<codeword_type> longest{ current_string_seen, seen_previously.value};
			return longest; 


			
		}



};	

template <typename dict_type, typename codeword_type> class LZW_Decode_Dictionary {
	public:
		dict_type dictionary;

		LZW_Decode_Dictionary(dict_type dict): dictionary(dict){};
};	
