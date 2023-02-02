#include <iostream>
#include "dictionary.hh"
#include <unordered_map>

template <typename codeword_type> class Std_Encode_Dictionary: private LZWDictionary<codeword_type>{
	private:
		std::unordered_map<std::string, codeword_type> dictionary;
	public:
		typedef LZWDictionary<codeword_type>::Dict_Entry Std_Dict_Entry;
		typedef LZWDictionary<codeword_type>::Codeword_Found Codeword_Found;
		Std_Encode_Dictionary(): LZWDictionary<codeword_type>(){};
		Std_Dict_Entry find_longest_in_dict(std::istream& input) override{

				char f = input.get();
				std::cout << f;
				return Std_Dict_Entry{ "foo", 1};
		}
	

		void add_string(std::string str, codeword_type codeword) override{
			dictionary[str] = codeword;
		}

		std::string str_of(codeword_type codeword) const override {
			std::cout << codeword;
			return "foo";
		}

		codeword_type code_of(std::string str, unsigned len) const override{
			std::cout << len;
			auto lookup = dictionary.find(str);
			return lookup->second;
		}

};

