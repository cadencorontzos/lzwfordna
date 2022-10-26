#include "lzw.hh"
#include "bitio.hh"

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

void outputCodeword( int codeword, int codeword_size , BitOutput& bit_output){
    for (int i = codeword_size-1; i>=0; --i){
        bit_output.output_bit((codeword>>i)&1);
    }
}

void outputChar( char character,  BitOutput& bit_output){
    for (int i = CHAR_BIT-1; i>=0; --i){
        bit_output.output_bit((character>>i)&1);
    }
}

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
  
            outputCodeword(code, codeword_size, bit_output);
            outputChar(K, bit_output);
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
        
        // std::cout << "dsdfsd" << code << std::endl;
        // std::cout << 
        std::cout << currentBlock << std::endl;
        std::cout << "code left over" << std::endl;
        for (int i = codeword_size-1; i>=0; --i){
            bit_output.output_bit((code>>i)&1);
            std::cout<< ' ' << ((code>>i)&1) ;

        }
        outputChar(back, bit_output);


    }
    outputCodeword(256, codeword_size, bit_output);
}

static int get_codeword(int code_size, BitInput* bit_input){
    int b = 0;
    for (int i= code_size-1; i >=0; i--){
        b |= (bit_input->input_bit()<<i);
        // std::cout << b << std::endl;
    }
    return b;
}
    // Decode an input stream, and write to output stream
void LZW::decode(std::ifstream* input, std::ofstream* output){



    std::unordered_map<int, std::string> dictionary;

    for (int i = 0; i < 256; ++i){
        std::string str1(1, char(i));
        dictionary[i] = str1;
    }

    int code_size = 9;
    int codeword = 258;
    int codewordFound;
    int max_codeword_size = 1<<9;
    bool outputBoth;
    char nextByte;
    // std::cout << max_codeword_size;

    BitInput bit_input(*input);
    // std::cout << "here";
    // int currentBlock = get_codeword(code_size, &bit_input);
    codewordFound = get_codeword(code_size, &bit_input);
    while(codewordFound!=256){
        outputBoth = false;
        // int decodedPart = dictionary[codeword];
        if((*input).eof()){
            break;
        }

        nextByte = char(get_codeword(8, &bit_input));

        if (codewordFound==257){
            (*output) << nextByte;
            break;
        }

        outputBoth = true;
        // std::cout << " _________________________________________" << std::endl;
        // std::cout << "codeword = " << codeword << std::endl;
        // std::cout << "I found this codeword : "<< codewordFound << std::endl; //debug
        auto decodedCodeword = dictionary.find(codewordFound);
        // std::cout << "Which maps to this string: " << decodedCodeword->second << std::endl;//debug

        // std::cout << "and this byte : " << nextByte  << std::endl;//debug

        // std::cout << "I added " << codeword <<  "->" << decodedCodeword->second  << nextByte << " to the dictionary" << std::endl;//debug

        (*output) << decodedCodeword->second << nextByte;
        // std::cout << " _________________________________________" << std::endl;
        
        dictionary[codeword] = decodedCodeword->second + nextByte;
        codeword+=1;

        if (codeword == max_codeword_size){
            code_size += 1;
            max_codeword_size <<= 1;
        }
        
    
        codewordFound = get_codeword(code_size, &bit_input);
    
    }

    // if(!outputBoth){
    //     auto decodedCodeword = dictionary.find(codewordFound);
    //     std::cout  << decodedCodeword->second << std::endl;
    //     (*output) << decodedCodeword->second;
        
    // }

    // std::cout << codewordFound << std::endl;
    // std::cout << dictionary[codewordFound] << std::endl;

    // std::cout << nextByte;
}
