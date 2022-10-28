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

LZW::LZW() = default;

LZW::~LZW() = default;

void LZW::encode(std::istream& input, std::ostream& output){
    
    BitOutput bit_output(output);
    std::unordered_map<std::string, int> dictionary;

    for (int i = 0; i < 256; ++i){
        std::string str1(1, char(i));
        dictionary[str1] = i;
    }
 
    char K;
    int codeword = 258;
    std::string str_K;
    int codeword_size = 9;
    int max_codeword_size = 1<<9;
    std::string currentBlock = "";

    K = input.get();
    while(K != -1){
        if (codeword == max_codeword_size){
            codeword_size += 1;
            max_codeword_size<<= 1;
        }
        if (dictionary.find(currentBlock + K) != dictionary.end()){
            currentBlock = currentBlock +K;
        }
        else{
            int code = dictionary[currentBlock];
            bit_output.output_n_bits(code, codeword_size);
            bit_output.output_n_bits((int) K, CHAR_BIT);
            dictionary[currentBlock + K] = codeword;
            codeword += 1;
            currentBlock = "";
        }
        K = input.get();
    }

    if (currentBlock !=""){
        std::cout << currentBlock;
        char back = currentBlock.back();
        currentBlock.pop_back();
        int code;
        if(currentBlock.length() == 0){
            code = 257;
        }else{
            code = dictionary[currentBlock];
        }
        
        bit_output.output_n_bits(code, codeword_size);
        bit_output.output_n_bits((int) back, CHAR_BIT);

    }
    bit_output.output_n_bits(256, codeword_size);
}



void LZW::decode(std::istream& input, std::ostream& output){

    std::unordered_map<int, std::string> dictionary;

    for (int i = 0; i < 256; ++i){
        std::string str1(1, char(i));
        dictionary[i] = str1;
    }

    int code_size = 9;
    int codeword = 258;
    int codewordFound;
    int max_codeword_size = 1<<9;
    char nextByte;
    BitInput bit_input(input);

    codewordFound = bit_input.read_n_bits(code_size);

    while(codewordFound!=256){
     

        if(input.eof()){
            break;
        }

        nextByte = char(bit_input.read_n_bits(8));

        if (codewordFound==257){
            output << nextByte;
            break;
        }
        auto decodedCodeword = dictionary.find(codewordFound);
        output << decodedCodeword->second << nextByte;    
        dictionary[codeword] = decodedCodeword->second + nextByte;
        codeword+=1;

        if (codeword == max_codeword_size){
            code_size += 1;
            max_codeword_size <<= 1;
        }
        
    
        codewordFound = bit_input.read_n_bits(code_size);
    
    }

}
