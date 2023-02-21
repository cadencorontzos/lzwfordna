#include <iostream>
#include <string>
#include <cassert>


// Parent class
// Children should define LZW_Encode_Dictionary and LZW_Decode_Dictionary
template <typename codeword_type> class LZWDictionary {

	public:

		LZWDictionary() = default;
		~LZWDictionary() = default;

		// takes the input stream, pointer into stream, and input_size
		// returns the longest string that is present in the dicitonary, starting from the pointer. Subject to size constraints
		virtual int find_longest_in_dict(const char* , int , int ){
			assert(0);
		}

		virtual int find_longest_in_dict(const char* , const char*){
			assert(0);
		}

		virtual void add_string(std::string , codeword_type ){
			assert(0);
		}
		virtual void add_string(const char*, int , codeword_type ){
			assert(0);
		}

		virtual codeword_type code_of(std::string , unsigned ) const {
			assert(0);
		}
		virtual codeword_type code_of(const char*, unsigned ) const{
			assert(0);
		}


		virtual std::string str_of(codeword_type ) const {
			assert(0);
		}


		virtual void load_starting_dictionary() = 0;

};

template <typename codeword_type> class CW_Tracker{
	public:
		codeword_type current_codeword;
		const int EOF_CODEWORD;
		int bits_per_codeword;
		CW_Tracker(unsigned starting_codeword, unsigned eof, unsigned starting_codeword_size): 
			current_codeword(starting_codeword), 
			EOF_CODEWORD(eof), 
			bits_per_codeword(starting_codeword_size){};
		~CW_Tracker() = default;
		virtual codeword_type get_next_codeword() = 0;
};
