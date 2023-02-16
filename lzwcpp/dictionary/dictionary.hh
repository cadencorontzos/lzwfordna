#include <iostream>
#include <string>
#include <cassert>

template <typename codeword_type> class LZWDictionary {
	public:
		LZWDictionary() = default;
		~LZWDictionary() = default;

		// takes the input stream, pointer into stream, and input_size
		// returns the longest string that is present in the dicitonary, starting from the pointer. Subject to size constraints
		virtual int find_longest_in_dict(const char* , int , int ){
			assert(0);
		}

		virtual void add_string(std::string str, codeword_type codeword) = 0;

		virtual codeword_type code_of(std::string , unsigned ) const {
			assert(0);
		}

		virtual std::string str_of(codeword_type ) const {
			assert(0);
		}

};
