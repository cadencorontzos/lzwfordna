#include "lzw.hh"
#include <iostream>
#include <string>
#include <fstream>
#include <climits>
#include "./dictionary/direct_mapped_dict.hh"

void LZW::encode(const char* input_file, int file_size, std::ostream& output){
   
    // initialize starter dictionary
	LZW_Encode_Dictionary dictionary;
	dictionary.load_starting_dictionary();

    BitOutput bit_output(output);

    // the current codeword we are using, and the size of the codewords
    // each time we use a codeword we will have to increment so all codewords are unique
    // TODO: need to make sure our codewords don't go over the max size of codeword_type
	Codeword_Helper codeword_helper;
    codeword_type codeword = codeword_helper.get_next_codeword();

    // the pieces of the file we are reading
    // current string seen is a string that we've seen before (it is in the dictionary), next_character is the following character that we are looking at

	char next_character;
	int entry; 
	int code;
	const char* end_of_input = input_file + file_size;
	int i = 0;
	while(true)
	{

		i++;
		entry = dictionary.find_longest_in_dict(input_file, end_of_input);
		code = dictionary.code_of(input_file, entry);
		if((input_file + entry)>= end_of_input){
			break;
		}
		next_character = input_file[entry];
			
		bit_output.output_n_bits(code, codeword_helper.bits_per_codeword);
		bit_output.output_n_bits(static_cast<uint8_t>(next_character), CHAR_BIT);
		dictionary.add_string(input_file, entry+1, codeword);
		codeword = codeword_helper.get_next_codeword();
	   
		input_file+=entry+1;
		entry = 0;

	}
    // output special eof character
    bit_output.output_n_bits(codeword_helper.EOF_CODEWORD, codeword_helper.bits_per_codeword);

    // after we've encoded, we either have 
    // no current block (case 0)
    // we have a current block that is a single character (case 1)
    // otherwise we have a current block > 1 byte (default)
    switch (entry){
    case 0:
        bit_output.output_bit(false);
        bit_output.output_bit(false);
        break;
    case 1:
        bit_output.output_bit(false);
        bit_output.output_bit(true);
        bit_output.output_n_bits(static_cast<uint8_t>(input_file[0]), CHAR_BIT);
        break;
    default:
        bit_output.output_bit(true);
        bit_output.output_bit(true);

        bit_output.output_n_bits(code, codeword_helper.bits_per_codeword);
        break;
    }
}



void LZW::decode(const char* input, std::ostream& output){

    // starting dictionary
    LZW_Decode_Dictionary dictionary;
	dictionary.load_starting_dictionary();

	Codeword_Helper codeword_helper;
    codeword_type codeword = codeword_helper.get_next_codeword();
    int codeword_found;
    char next_byte;
    BitInput bit_input(input);

    // assume the file isn't empty
    codeword_found = bit_input.read_n_bits(codeword_helper.bits_per_codeword);
    while(codeword_found!=codeword_helper.EOF_CODEWORD){

        next_byte = char(bit_input.read_n_bits(CHAR_BIT));

        // look up the codeword in the dictionary
		std::string decodedCodeword = dictionary.str_of(codeword_found);
		std::string new_string = decodedCodeword+ next_byte;
        
		
        // output what we had in the dictionary and the byte following
        output << new_string; 

        // add this new sequence to our dictionary   
        dictionary.add_string(new_string, codeword);
        codeword+=1;
        codeword_found = bit_input.read_n_bits(codeword_helper.bits_per_codeword);
    }
    
    // the two bits after eof are a code for the last portion of the encoded file
    // either there is an extra codeword(1), extra byte (3), or nothing left (0)
    int whats_left = bit_input.read_n_bits(2);
    switch(whats_left){
    case 1:
        output << char(bit_input.read_n_bits(CHAR_BIT));
        break;
    case 3:
        int last_codeword = bit_input.read_n_bits(codeword_helper.bits_per_codeword);
        auto decoded_codeword = dictionary.str_of(last_codeword);
        output << decoded_codeword; 
        break;
    }

}
