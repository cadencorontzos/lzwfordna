
#include <iostream>
#include "dictionary.hh"
#include <unordered_map> 
#include <memory>
#include <cmath>
#include <climits>

constexpr uint8_t MAX_STRING_LENGTH = 10;
typedef uint16_t codeword_type;
const codeword_type MAX_CODEWORD = static_cast<codeword_type>((1<<(sizeof(codeword_type)*CHAR_BIT)) -1);

class LZW_Encode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		using index_type = uint32_t;
		const int INDEX_BITS = CHAR_BIT*sizeof(index_type);
		const int CODEWORD_SIZE = INDEX_BITS/2;
		const int MAX_CODEWORD = (1<<CODEWORD_SIZE)-1;
		std::unordered_map<char, int> f{
			{'A', 0},
			{'T', 1},
			{'C', 2},
			{'G', 3}
		};

		index_type map_str(std::string str) const{
			index_type result = 0;
			int len = str.length();
			for(int i = 0; i < len;i++){
				auto entry = f.find(str[i]);
				result = (result<<2) + entry->second;
			}
			return result;
		}

		std::unordered_map<std::string, codeword_type> longer_than_max;
		int starting_codeword;
		std::unordered_map<std::string, codeword_type>::const_iterator end;	
		std::array<std::unique_ptr<codeword_type[]>, MAX_STRING_LENGTH> dictionary; 
		bool empty;
	public:

		LZW_Encode_Dictionary() : LZWDictionary<codeword_type>(){ 
			empty = false;
			end = longer_than_max.cend();
			for( int i = 0; i < MAX_STRING_LENGTH; i++){
				int size = pow(4, i+1);
				dictionary[i] = std::unique_ptr<codeword_type[]>((codeword_type*)calloc(size, sizeof(codeword_type)));
				if(!dictionary[i]){
					std::cout << "Memory Failure. Exiting...." << std::endl;
					abort();
				}
			}
		} 

		codeword_type code_of(std::string str, unsigned len) const override{
			if (len >= MAX_STRING_LENGTH){
				auto lookup = longer_than_max.find(str);
				if(lookup == end){ return 0;}
				return lookup->second;
			}else{
				codeword_type lookup = (dictionary[len])[map_str(str)];
				return lookup;
			}
		}
	
		int find_longest_in_dict(const char* input, int input_start, int input_size) override{
			char next_character;
			int current_index = input_start;
			std::string current_string_seen = "";
			std::string string_seen_plus_new_char;
			int length = 0;
			int entry = 0;
			while(current_index < input_size) {
				next_character = input[current_index];
				length++;
				string_seen_plus_new_char = current_string_seen + next_character;
				entry = code_of(string_seen_plus_new_char, length);
				if (entry!= 0){
					current_string_seen = string_seen_plus_new_char;
				}else{
					return length-1;
				}
				current_index ++;

			}
			if(current_string_seen == ""){
				return 0;
			}
			return length;
		}

		void add_string(std::string str, codeword_type codeword) override{
			if(empty){return;}
			if(codeword ==(1<< CODEWORD_SIZE)-1){
				empty = true;
			}
			if(str.length() < MAX_STRING_LENGTH){
				(dictionary[str.length()])[map_str(str)] = codeword;
			}else{
				longer_than_max[str] = codeword;
			}
		}

		void load_starting_dictionary() override{
			add_string("A", 1);
			add_string("T", 2);
			add_string("C", 3);
			add_string("G", 4);
		}

};

class LZW_Decode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		static constexpr int CODEWORD_SIZE= CHAR_BIT*sizeof(codeword_type);
		const int MAX_CODEWORD = (1<<CODEWORD_SIZE)-1;
		bool empty;
	public:
		std::array<std::string, (1<< CODEWORD_SIZE)> dictionary; 
		LZW_Decode_Dictionary() : LZWDictionary<codeword_type>(){empty = false;}
	
		void add_string(std::string str, codeword_type codeword) override{
			if(empty){return;}
			if(codeword == (1<< CODEWORD_SIZE)-1){
				empty = true;
			}
			dictionary[codeword] = str;
		}

		std::string str_of(codeword_type codeword) const override {
			assert(codeword < (1<< CODEWORD_SIZE));
			return dictionary[codeword];
		}

		void load_starting_dictionary() override{
			add_string("A", 1);
			add_string("T", 2);
			add_string("C", 3);
			add_string("G", 4);
		}
};


class Codeword_Helper: public CW_Tracker<codeword_type>{
	public:
		Codeword_Helper() : CW_Tracker<codeword_type>(6, 5, sizeof(codeword_type)*CHAR_BIT){};
		codeword_type get_next_codeword(){
			if(current_codeword == MAX_CODEWORD){
				return MAX_CODEWORD;
			}
			else{
				return current_codeword++;
			}
		}	
};

