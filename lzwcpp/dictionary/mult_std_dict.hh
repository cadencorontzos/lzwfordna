#include <iostream>
#include "dictionary.hh"
#include <unordered_map> 
#include <memory>
#include <cmath>
#include <climits>
#include <limits>

typedef uint32_t codeword_type;
const codeword_type MAX_CODEWORD = std::numeric_limits<codeword_type>::max();
const int CODEWORD_SIZE = std::numeric_limits<codeword_type>::digits;
const int MAX_STRING_LENGTH = 10;

// Std Mult dict Encode
//
// use an array of unordered map to track codewords
//
class LZW_Encode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		// array of dictionaries
		// we need one for every string length up to our max, plus another for all strings longer than that max
		// we also don't want weird indexing so we will have one dict at the bottom that is empty
		std::array<std::unordered_map<std::string, codeword_type>, MAX_STRING_LENGTH+2> dictionary_array; 
		std::array<std::unordered_map<std::string, codeword_type>::const_iterator, MAX_STRING_LENGTH+2> dictionary_ends;	

		// track when we run out of codewords
		bool empty;

		int len_to_index(unsigned len) const{
			if(len <= MAX_STRING_LENGTH){
				return len;
			}
			return MAX_STRING_LENGTH;
		}
	public:
		LZW_Encode_Dictionary() : 
			LZWDictionary<codeword_type>(),
			empty(false){
				for(unsigned i = 0; i < dictionary_ends.size(); i++){
					dictionary_ends[i] = dictionary_array[i].cend();
				} 
			}

		codeword_type code_of(const char* input, unsigned len) const override{
	
			std::string str(input, len);
			auto lookup = dictionary_array[len_to_index(len)].find(str);
			if(lookup == dictionary_ends[len_to_index(len)]){ return 0;}
			return lookup->second;
		}
	
		int find_longest_in_dict(const char* input, const char* end_of_input) override{
			int length = 0;
			int entry = 0;
			while(input+length < end_of_input) {
				length++;
				entry = code_of(input, length);
				// if entry is non zero, it means we have seen that string before
				if (entry == 0){
					return length-1;
				}
			}
			if(length == 0){
				return 0;
			}
			return length;
		}

		void add_string(const char* input, unsigned len, codeword_type codeword) override{
			// we want to prevent overflow, so when we reached our limit,
			// allow this last addition then declare the dict empty.
			if(empty){return;}
			if(codeword == MAX_CODEWORD){
				empty = true;
			}
			std::string str(input, len);
			dictionary_array[len_to_index(len)][str] = codeword;
		}

		void load_starting_dictionary() override{
			add_string("A", 1, 1);
			add_string("T", 1, 2);
			add_string("C", 1, 3);
			add_string("G", 1, 4);
		}

};

// std Decode
// 
//
class LZW_Decode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		// dictionary and end of dictionary 
		std::unordered_map<codeword_type, std::string> dictionary; 
		std::unordered_map<codeword_type, std::string>::const_iterator end;	
		bool empty;
	public:
		LZW_Decode_Dictionary() : 
			LZWDictionary<codeword_type>(), 
			end(dictionary.cend()), 
			empty(false){}
	
		void add_string(std::string str, codeword_type codeword) override{
			if(empty){return;}
			if(codeword == MAX_CODEWORD){
				empty = true;
			}
			dictionary[codeword] = str;
		}

		std::string str_of(codeword_type codeword) const override {
			return dictionary.find(codeword)->second;
		}

		void load_starting_dictionary() override{
			add_string("A", 1);
			add_string("T", 2);
			add_string("C", 3);
			add_string("G", 4);
		}
};

// Codeword_Helper for mult_dicts:
//
// track current codeword and use the minumum number of bits required to display said codeword
//
class Codeword_Helper: public CW_Tracker<codeword_type>{
	private:
		unsigned int current_max_cw;
	public:
		Codeword_Helper() : CW_Tracker<codeword_type>(6, 5, 3), current_max_cw(1<<bits_per_codeword){};
		codeword_type get_next_codeword(){
			if(current_codeword == MAX_CODEWORD){
				return MAX_CODEWORD;
			}
			if (current_codeword == current_max_cw){
				bits_per_codeword++;
				current_max_cw = (1<<bits_per_codeword);
			}
			return current_codeword++;
		}	
};

