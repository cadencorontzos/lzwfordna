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
    auto end = dictionary.cend();
 
    BitOutput bit_output(output);

    // the current codeword we are using, and the size of the codewords
    // each time we use a codeword we will have to increment so all codewords are unique
    // TODO: need to make sure our codewords don't go over the max size of codeword_type
    int codeword = STARTING_CODEWORD;
    int codeword_size = STARTING_CODE_SIZE;
    int max_codeword_size = 1<<STARTING_CODE_SIZE;

    // the pieces of the file we are reading
    // current block is a string that we've seen before (its in the dictionary), next_character is the following character that we are looking at
    std::string currentBlock = "";
    char next_character;

    next_character = input.get();

    while(next_character != EOF){

        // increment the codword size if the current codeword becomes too large
        if (codeword == max_codeword_size){
            codeword_size += 1;
            max_codeword_size<<= 1;
        }

        // if we've already seen the sequence, keep going
        // TODO: use cend() and save this iterator
        if (dictionary.find(currentBlock + next_character) != end ){
            currentBlock = currentBlock + next_character;
        }
        else{

            // lookup the current block in the dictionary and output it, along with the new character
            // shouldn't look up again
            int code = dictionary[currentBlock];
            bit_output.output_n_bits(code, codeword_size);
            // TODO: static cast to unsigned (uint8t)
            bit_output.output_n_bits((int) next_character, CHAR_BIT);

            // add this new sequence to our dictionary
            dictionary[currentBlock + next_character] = codeword;
            codeword += 1;
            currentBlock = "";
        }
        next_character = input.get();
    }

    // output special eof character
    bit_output.output_n_bits(EOF_CODEWORD, codeword_size);

    // after we've encoded, we either have 
    // no current block (case 0)
    // we have a current block that is a single character (case 1)
    // otherwise we have a current block > 1 byte (default)
    switch (currentBlock.length()){
    case 0:
        bit_output.output_bit(false);
        bit_output.output_bit(false);
        break;
    case 1:
        bit_output.output_bit(false);
        bit_output.output_bit(true);
        bit_output.output_n_bits((int) currentBlock[0], CHAR_BIT);
        break;
    default:
        bit_output.output_bit(true);
        bit_output.output_bit(true);

        int code = dictionary[currentBlock];
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
    int codeword = STARTING_CODEWORD;
    int codewordFound;
    int max_codeword_size = 1<<STARTING_CODE_SIZE;
    char nextByte;
    BitInput bit_input(input);

    // assume the file isn't empty
    codewordFound = bit_input.read_n_bits(code_size);
    while(codewordFound!=EOF_CODEWORD){

        nextByte = char(bit_input.read_n_bits(CHAR_BIT));

        // look up the codeword in the dictionary
        auto decodedCodeword = dictionary.find(codewordFound);
        
        // output what we had in the dictionary and the byte following
        output << decodedCodeword->second << nextByte; 

        // add this new sequence to our dictionary   
        dictionary[codeword] = decodedCodeword->second + nextByte;
        codeword+=1;

        // increment the codeword size if needed
        if (codeword == max_codeword_size){
            code_size += 1;
            max_codeword_size <<= 1;
        }
        
        codewordFound = bit_input.read_n_bits(code_size);
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
        auto decodedCodeword = dictionary.find(last_codeword);
        output << decodedCodeword->second; 
        break;
    }

}
