#include "lzw.hh"

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <unordered_map>
#include <climits>

namespace fs = std::filesystem;

void LZW::encode(std::istream& input, std::ostream& output){
    
    // initialize starter dictionary
    std::unordered_map<std::string, codeword_type> dictionary;
    for (int i = 0; i < STARTING_DICT_SIZE; ++i){
        std::string str1(1, char(i));
        dictionary[str1] = static_cast<codeword_type>(i);
    }
    auto not_in_dictionary = dictionary.cend();

    BitOutput bit_output(output);

    // the current codeword we are using, and the size of the codewords
    // each time we use a codeword we will have to increment so all codewords are unique
    // TODO: need to make sure our codewords don't go over the max size of codeword_type
    codeword_type codeword = STARTING_CODEWORD;
    int codeword_size = STARTING_CODE_SIZE;
    codeword_type biggest_possible_codeword = 1<<STARTING_CODE_SIZE;

    // the pieces of the file we are reading
    // current string seen is a string that we've seen before (it is in the dictionary), next_character is the following character that we are looking at
    std::string current_string_seen = "";
    char next_character;

    next_character = input.get();

    while(next_character != EOF){

        // increment the codword size if the current codeword becomes too large
        if (codeword == biggest_possible_codeword){
            codeword_size += 1;
            biggest_possible_codeword<<= 1;
        }

        // if we've already seen the sequence, keep going
        std::string string_seen_plus_new_char = current_string_seen + next_character;
		// save this iterator`
        if (dictionary.find(string_seen_plus_new_char) != not_in_dictionary ){
            current_string_seen = string_seen_plus_new_char;
        }
        else{

            // lookup the current block in the dictionary and output it, along with the new character
            // shouldn't look up again
            int code = dictionary[current_string_seen];
            bit_output.output_n_bits(code, codeword_size);
            bit_output.output_n_bits(static_cast<uint8_t>(next_character), CHAR_BIT);

            // add this new sequence to our dictionary
            dictionary[string_seen_plus_new_char] = codeword;
            codeword += 1;
            current_string_seen = "";
        }
        next_character = input.get();
    }

    // output special eof character
    bit_output.output_n_bits(EOF_CODEWORD, codeword_size);

    // after we've encoded, we either have 
    // no current block (case 0)
    // we have a current block that is a single character (case 1)
    // otherwise we have a current block > 1 byte (default)
    switch (current_string_seen.length()){
    case 0:
        bit_output.output_bit(false);
        bit_output.output_bit(false);
        break;
    case 1:
        bit_output.output_bit(false);
        bit_output.output_bit(true);
        bit_output.output_n_bits(static_cast<uint8_t>(current_string_seen[0]), CHAR_BIT);
        break;
    default:
        bit_output.output_bit(true);
        bit_output.output_bit(true);

        int code = dictionary[current_string_seen];
        bit_output.output_n_bits(code, codeword_size);
        break;
    }
}



void LZW::decode(std::istream& input, std::ostream& output){

    // starting dictionary
    std::unordered_map<codeword_type, std::string> dictionary;
    for (int i = 0; i < STARTING_DICT_SIZE; ++i){
        std::string str1(1, char(i));
        dictionary[static_cast<codeword_type>(i)] = str1;
    }

    int code_size = STARTING_CODE_SIZE;
    codeword_type codeword = STARTING_CODEWORD;
    int codeword_found;
    codeword_type biggest_possible_codeword = 1<<STARTING_CODE_SIZE;
    char next_byte;
    BitInput bit_input(input);

    // assume the file isn't empty
    codeword_found = bit_input.read_n_bits(code_size);
    while(codeword_found!=EOF_CODEWORD){

        next_byte = char(bit_input.read_n_bits(CHAR_BIT));

        // look up the codeword in the dictionary
        auto decodedCodeword = dictionary.find(codeword_found);
        
        // output what we had in the dictionary and the byte following
        output << decodedCodeword->second << next_byte; 

        // add this new sequence to our dictionary   
        dictionary[codeword] = decodedCodeword->second + next_byte;
        codeword+=1;

        // increment the codeword size if needed
        if (codeword == biggest_possible_codeword){
            code_size += 1;
            biggest_possible_codeword <<= 1;
        }
        
        codeword_found = bit_input.read_n_bits(code_size);
    }
    
    // the two bits after eof are a code for the last portion of the encoded file
    // either there is an extra codeword(1), extra byte (3), or nothing left (0)
    int whats_left = bit_input.read_n_bits(2);
    switch(whats_left){
    case 1:
        output << char(bit_input.read_n_bits(CHAR_BIT));
        break;
    case 3:
        int last_codeword = bit_input.read_n_bits(code_size);
        auto decoded_codeword = dictionary.find(last_codeword);
        output << decoded_codeword->second; 
        break;
    }

}
