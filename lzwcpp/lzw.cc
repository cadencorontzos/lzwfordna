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

void LZW::output_n_bits(int bits, int n , BitOutput& bit_output){
    for (int i = n-1; i>=0; --i){
        bit_output.output_bit((bits>>i)&1);
    }
}

int LZW::read_n_bits(int n, BitInput& bit_input){
    int b = 0;
    for (int i= n-1; i >=0; i--){
        b |= (bit_input.input_bit()<<i);
    }
    return b;
}

LZW::LZW() = default;

LZW::~LZW() = default;

void LZW::encode(std::ifstream& input, std::ofstream& output){
    
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
            output_n_bits(code, codeword_size, bit_output);
            output_n_bits((int) K, CHAR_BIT, bit_output);
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
        
        output_n_bits(code, codeword_size, bit_output);
        output_n_bits((int) back, CHAR_BIT, bit_output);

    }
    output_n_bits(256, codeword_size, bit_output);
}



void LZW::decode(std::ifstream& input, std::ofstream& output){

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

    codewordFound = read_n_bits(code_size, bit_input);

    while(codewordFound!=256){
     

        if(input.eof()){
            break;
        }

        nextByte = char(read_n_bits(8, bit_input));

        if (codewordFound==257){
            output << nextByte;
            break;
        }

        // std::cout << " _________________________________________" << std::endl;
        // std::cout << "codeword = " << codeword << std::endl;
        // std::cout << "I found this codeword : "<< codewordFound << std::endl; //debug
        auto decodedCodeword = dictionary.find(codewordFound);
        // std::cout << "Which maps to this string: " << decodedCodeword->second << std::endl;//debug

        // std::cout << "and this byte : " << nextByte  << std::endl;//debug

        // std::cout << "I added " << codeword <<  "->" << decodedCodeword->second  << nextByte << " to the dictionary" << std::endl;//debug

        output << decodedCodeword->second << nextByte;
        // std::cout << " _________________________________________" << std::endl;
        
        dictionary[codeword] = decodedCodeword->second + nextByte;
        codeword+=1;

        if (codeword == max_codeword_size){
            code_size += 1;
            max_codeword_size <<= 1;
        }
        
    
        codewordFound = read_n_bits(code_size, bit_input);
    
    }

}
