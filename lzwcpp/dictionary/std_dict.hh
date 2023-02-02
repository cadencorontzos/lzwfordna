#include <iostream>
#include "dictionary.hh"
#include <unordered_map>

template <typename codeword_type> class Std_Encode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		std::unordered_map<std::string, codeword_type> dictionary;
		std::unordered_map<std::string, codeword_type>::const_iterator end;	
	public:
		typedef LZWDictionary<codeword_type>::Dict_Entry Std_Dict_Entry;
		typedef LZWDictionary<codeword_type>::Codeword_Found Codeword_Found;
		Std_Encode_Dictionary(): LZWDictionary<codeword_type>(){ end = dictionary.cend();};
		Std_Dict_Entry find_longest_in_dict(std::istream& input) override{
			char next_character = input.get();

			std::string current_string_seen = "";
			std::string string_seen_plus_new_char;
			auto seen_previously = end;
			while( next_character != EOF){

				string_seen_plus_new_char = current_string_seen + next_character;
				auto entry = dictionary.find(string_seen_plus_new_char);
				if (entry != end){
					current_string_seen = string_seen_plus_new_char;
					seen_previously = entry;
				}else{
					Std_Dict_Entry longest{ current_string_seen, seen_previously->second};
					input.putback(next_character);
					return longest;

				}
				next_character = input.get();

			}
			if(current_string_seen == ""){



				return Std_Dict_Entry{ current_string_seen, 0};
			}
			Std_Dict_Entry longest{ current_string_seen, seen_previously->second};
			return longest;


		}
		
	

		void add_string(std::string str, codeword_type codeword) override{
			dictionary[str] = codeword;
		}

		std::string str_of(codeword_type codeword) const override {
			int f = codeword+1;
			f+=1;
			return "foo";
		}

		codeword_type code_of(std::string str, unsigned len) const override{
			int f = len +1;
			f+=1;
			auto lookup = dictionary.find(str);
			return lookup->second;
		}

};

template <typename codeword_type> class Std_Decode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		std::unordered_map< codeword_type, std::string> dictionary;
		std::unordered_map< codeword_type, std::string>::const_iterator end;	
	public:
		typedef LZWDictionary<codeword_type>::Dict_Entry Std_Dict_Entry;
		typedef LZWDictionary<codeword_type>::Codeword_Found Codeword_Found;
		Std_Decode_Dictionary(): LZWDictionary<codeword_type>(){ end = dictionary.cend();};
		Std_Dict_Entry find_longest_in_dict(std::istream& input) override{
			input.get();
			Std_Dict_Entry longest;
			return longest;
		}
		
	

		void add_string(std::string str, codeword_type codeword) override{
			dictionary[codeword] = str;
		}

		std::string str_of(codeword_type codeword) const override {
			auto lookup = dictionary.find(codeword);
			return lookup->second;
		}

		codeword_type code_of(std::string str, unsigned len) const override{
			std::cout << str;
			return len;
		}

};

