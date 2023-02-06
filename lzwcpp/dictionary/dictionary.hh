
#include <iostream>
#include <string>
#include <cassert>

template <typename codeword_type> class LZWDictionary {
	public:
		struct Dict_Entry {
			std::string str;
			codeword_type codeword;
		};
		struct Codeword_Found{

			codeword_type value;
			bool exists;
		};
		LZWDictionary() = default;
		~LZWDictionary() = default;


		virtual Dict_Entry find_longest_in_dict(std::istream& ){
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
