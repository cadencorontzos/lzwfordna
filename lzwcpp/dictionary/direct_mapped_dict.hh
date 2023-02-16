
#include <iostream>
#include "dictionary.hh"
#include <unordered_map> 
#include <memory>
#include <cmath>

template <typename codeword_type = uint16_t> class Direct_Mapped_Encode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		using index_type = uint32_t;
		const int CODEWORD_SIZE=16;

		const int MAX_CODEWORD = (1<<CODEWORD_SIZE)-1;
		const int INDEX_BITS = 32;
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
		
		bool empty;
	public:
		uint8_t MAX_STRING_LENGTH = 10;
		std::array<std::unique_ptr<codeword_type[]>, 10> dictionary; 
		typedef LZWDictionary<codeword_type>::Dict_Entry Dict_Entry; 
		typedef LZWDictionary<codeword_type>::Codeword_Found Codeword_Found; 
		Direct_Mapped_Encode_Dictionary() : LZWDictionary<codeword_type>(){ 


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
		Dict_Entry find_longest_in_dict(const char* input, int input_start, int input_size) override{
			char next_character ;
			int current_index = input_start;
			std::string current_string_seen = "";
			std::string string_seen_plus_new_char;
			codeword_type seen_previously = 0;
			int length = 0;
			int entry = 0;
			while(current_index < input_size) {
				next_character = input[current_index];
				length++;
				string_seen_plus_new_char = current_string_seen + next_character;
				if(length < MAX_STRING_LENGTH){
				 	entry = (dictionary[string_seen_plus_new_char.length()])[map_str(string_seen_plus_new_char)];
				}else{
					auto lookup = longer_than_max.find(string_seen_plus_new_char);
					
					if(lookup ==end){
						entry = 0;
					}else{
						entry = lookup->second;
					}
					
				}
				if (entry!= 0){
					current_string_seen = string_seen_plus_new_char;
					seen_previously = entry;
				}else{
					Dict_Entry longest{ current_string_seen, seen_previously};
					return longest;

				}
				current_index ++;
				

			}
			if(current_string_seen == ""){
				return Dict_Entry{ current_string_seen, 0};
			}
			Dict_Entry longest{ current_string_seen, seen_previously};
			return longest;


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


		codeword_type code_of(std::string str, unsigned len) const override{
			if (len >= MAX_STRING_LENGTH){
				auto lookup = longer_than_max.find(str);
				return lookup->second;
			}else{
				codeword_type lookup = (dictionary[len])[map_str(str)];
				return lookup;
			}
		}

};
template <typename codeword_type = uint16_t> class Direct_Mapped_Decode_Dictionary: private LZWDictionary<codeword_type>{
	private:

		static constexpr int CODEWORD_SIZE=16;

		const int MAX_CODEWORD = (1<<CODEWORD_SIZE)-1;
		const int INDEX_BITS = 32;
		bool empty;

	public:
		uint8_t MAX_STRING_LENGTH = 10;
		std::array<std::string, (1<< CODEWORD_SIZE)> dictionary; 
		Direct_Mapped_Decode_Dictionary() : LZWDictionary<codeword_type>(){empty = false;}
	

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
};
