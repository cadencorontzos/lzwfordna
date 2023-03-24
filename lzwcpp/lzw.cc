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

    // track our codewords with a helper
	Codeword_Helper codeword_helper;
    codeword_type codeword = codeword_helper.get_next_codeword();

	char next_character;
	int length_of_next_run; 
	int codeword_to_output;
	const char* end_of_input = input_file + file_size;
	while(true)
	{

		// find longest run of chars already in our dictionary 
		length_of_next_run = dictionary.find_longest_in_dict(input_file, end_of_input);

		// make sure we haven't passed EOF
		if((input_file + length_of_next_run)>= end_of_input){
			break;
		}

		// look up code of longest run
		codeword_to_output = dictionary.code_of(input_file, length_of_next_run);

		// output codeword	
		bit_output.output_n_bits(codeword_to_output, codeword_helper.bits_per_codeword);

		// output next character
		next_character = input_file[length_of_next_run];
		bit_output.output_n_bits(static_cast<uint8_t>(next_character), CHAR_BIT);

		// add the run we saw + the new character to our dict
		dictionary.add_string(input_file, length_of_next_run+1, codeword);

		codeword = codeword_helper.get_next_codeword();
		input_file+=length_of_next_run+1;
		length_of_next_run = 0;

	}
    // output special eof character
    bit_output.output_n_bits(codeword_helper.EOF_CODEWORD, codeword_helper.bits_per_codeword);

    // after we've encoded, we either have 
    // no current block (case 0)
    // we have a current block that is a single character (case 1)
    // otherwise we have a current block > 1 byte (default)
    switch (length_of_next_run){
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

		codeword_to_output = dictionary.code_of(input_file, length_of_next_run);
        bit_output.output_n_bits(codeword_to_output, codeword_helper.bits_per_codeword);
        break;
    }
}



void LZW::decode(const char* input, std::ostream& output){

    // load starting dictionary
    LZW_Decode_Dictionary dictionary;
	dictionary.load_starting_dictionary();

	// track our codewords with a helper
	Codeword_Helper codeword_helper;
    codeword_type codeword = codeword_helper.get_next_codeword();

    int codeword_found;
    char next_byte;
    BitInput bit_input(input);
    // assume the file isn't empty
	// read a codeword
    codeword_found = bit_input.read_n_bits(codeword_helper.bits_per_codeword);
    while(codeword_found!=codeword_helper.EOF_CODEWORD){
		
		// read the next character
        next_byte = char(bit_input.read_n_bits(CHAR_BIT));

        // look up the codeword in the dictionary and add on next character
		std::string decodedCodeword = dictionary.str_of(codeword_found);
		std::string new_string = decodedCodeword+ next_byte;
		
        // output the new string
        output << new_string; 

        // add this new string to our dictionary   
        dictionary.add_string(new_string, codeword);
		codeword = codeword_helper.get_next_codeword();
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
