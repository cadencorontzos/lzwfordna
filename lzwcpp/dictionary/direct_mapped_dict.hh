
#include <iostream>
#include "dictionary.hh"
#include <unordered_map> 
#include <memory>
#include <cmath>

template <typename codeword_type = uint16_t> class Direct_Mapped_Dictionary: private LZWDictionary<codeword_type>{
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
			std::cout << str << ":"<< result <<std::endl;

			return result;

		}

		int starting_codeword;
	public:
		uint8_t MAX_STRING_LENGTH = 10;
		std::array<std::unique_ptr<codeword_type[]>, 10> dictionary; 
		typedef LZWDictionary<codeword_type>::Dict_Entry Dict_Entry; 
		typedef LZWDictionary<codeword_type>::Codeword_Found Codeword_Found; 
		Direct_Mapped_Dictionary() : LZWDictionary<codeword_type>(){ 


			for( int i = 0; i < MAX_STRING_LENGTH; i++){
				int size = pow(4, i+1);
				dictionary[i] = std::unique_ptr<codeword_type[]>((codeword_type*)calloc(size, sizeof(codeword_type)));
				if(!dictionary[i]){
					std::cout << "Memory Failure. Exiting...." << std::endl;
					abort();


				}

			}


		} 
		Dict_Entry find_longest_in_dict(std::istream& input) override{
			char next_character = input.get();

			int length = 1;
			std::string current_string_seen = "";
			std::string string_seen_plus_new_char;
			codeword_type seen_previously = 0;
			while( next_character != EOF && length< 10){

				string_seen_plus_new_char = current_string_seen + next_character;
				int entry = (dictionary[string_seen_plus_new_char.length()])[map_str(string_seen_plus_new_char)];
				if (entry!= 0){
					current_string_seen = string_seen_plus_new_char;
					seen_previously = entry;
				}else{
					Dict_Entry longest{ current_string_seen, seen_previously};
					input.putback(next_character);
					return longest;

				}
				next_character = input.get();

			}
			if(current_string_seen == ""){



				return Dict_Entry{ current_string_seen, 0};
			}
			Dict_Entry longest{ current_string_seen, seen_previously};
			return longest;


		}
		
	

		void add_string(std::string str, codeword_type codeword) override{
			assert(str.length() < MAX_STRING_LENGTH);
			(dictionary[str.length()])[map_str(str)] = codeword;
		}


		codeword_type code_of(std::string str, unsigned len) const override{
			assert(len < MAX_STRING_LENGTH);
			codeword_type lookup = (dictionary[len])[map_str(str)];
			return lookup;
		}

};
