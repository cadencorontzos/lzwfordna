#include "lzw.hh"
#include "bitio.hh"
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

LZW::LZW() = default;


LZW::~LZW() = default;

CompressionStatistics* LZW::encode(std::string filename, std::string output_filename){
    
    std::ifstream input (filename, std::ios::binary);

    if (!input.is_open()){
        std::cout << "Unable to open " << filename << ".\n";
        return nullptr;
    }

    std::ofstream output;

    auto original_size =  fs::file_size(filename);

    output.open(output_filename, std::ios::binary);

    BitOutput bit_output(output);
    std::unordered_map<std::string, int> dictionary;

    for (int i = 0; i < 256; ++i){
        std::string str1(1, char(i));
        dictionary[str1] = i;
    }
    // std::cout << std::to_string(0) << std::endl;
    char K;
    int codeword = 256;
    std::string str_K;
    int codeword_size = 8;
    int max_codeword_size = 1<<8;
    // std::string f = "11";
    std::cout << "Encoding File " << filename << std::endl;
    // std::cout << ((dictionary.find(f))==dictionary.end())<< std::endl;
    K = input.get();
    // std::cout << K << std::endl;
    std::string currentBlock = "";
    int blockSize = 0;
    // output << "fuck";
    while(K != -1){
        
        // std::cout << "next character: "  << K << std::endl; // debug
        // If our table is full, expand
        if (codeword == max_codeword_size){
            codeword_size += 1;
            max_codeword_size<<= 1;
        }

        

        // If w+K is not new, build up w
        if (dictionary.find(currentBlock + K) != dictionary.end()){
            
            currentBlock = currentBlock +K;
            //std::cout << currentBlock << " is already in the dict" << std::endl;//debug
            // std::cout << "here:" << currentBlock << std::endl;
            blockSize++;

        }
        else{
            // std::cout << "I haven't seen this before :"  << currentBlock << " " << K << std::endl; //debug
            int code = dictionary[currentBlock];
            // std::cout << code << std::endl;
            blockSize = 0;
            // std::cout << "I'll output this codeword (in binary) :" ; //debug
            for (int i = codeword_size-1; i>=0; --i){
                bit_output.output_bit((code>>i)&1);
                // std::cout << ((code>>i)&1) ;//debug
                // std::cout<< ' ' << ((code>>i)&1) ;
    
            }
            // std::cout << std::endl << "and this character:" << K << "which in binary is " << std::endl;//debug
            for (int i = 7; i>=0; --i){
                bit_output.output_bit((K>>i)&1);
                // std::cout<< ' ' << ((K>>i)&1) ;//debug
            }
            dictionary[currentBlock + K] = codeword;
            codeword += 1;
            currentBlock = "";
        }

        // std::cout << char(K);
        K = input.get();
    }

    if (currentBlock !="" && dictionary.find(currentBlock ) != dictionary.end()){
        
        int code = dictionary[currentBlock];
        // std::cout << "dsdfsd" << code << std::endl;
        // std::cout << 
        std::cout << currentBlock << std::endl;
        std::cout << "code left over" << std::endl;
        for (int i = codeword_size-1; i>=0; --i){
            bit_output.output_bit((code>>i)&1);
            std::cout<< ' ' << ((code>>i)&1) ;
        }

    }else if (currentBlock!=""){

        int code = dictionary[currentBlock];
        
        std::cout << "new code at end "<< code << std::endl;
        blockSize = 0;
        // std::cout << code << std::endl;
        for (int i = codeword_size-1; i>=0; --i){
            bit_output.output_bit((code>>i)&1);
            // std::cout << ((code>>i)&1) ;
            // std::cout<< ' ' << ((code>>i)&1) ;

        }

    }



    CompressionStatistics* stats = new CompressionStatistics {true, 0.0, 0.0, 1, 1} ;
    return stats;
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
CompressionStatistics* LZW::decode(std::string filename, std::string output_filename){

    std::ifstream input (filename, std::ios::binary);

    // if file can't be opened, move on to the next argument
    if (!input.is_open()){
    
        std::cout << "Unable to open " << filename << ".\n";
        return nullptr;
    }

    std::ofstream output;


    output.open(output_filename, std::ios::binary);


    std::unordered_map<int, std::string> dictionary;

    for (int i = 0; i < 256; ++i){
        std::string str1(1, char(i));
        dictionary[i] = str1;
    }

    int code_size = 8;
    int codeword = 256;
    int codewordFound;
    int max_codeword_size = 1<<8;
    bool outputBoth;
    char nextByte;
    // std::cout << max_codeword_size;

    BitInput bit_input(input);
    // std::cout << "here";
    // int currentBlock = get_codeword(code_size, &bit_input);
    while(!input.eof()){


        if (codeword == max_codeword_size){
            code_size += 1;
            max_codeword_size <<= 1;
        }
        
        outputBoth = false;
        // int decodedPart = dictionary[codeword];
        codewordFound = get_codeword(code_size, &bit_input);
        if(input.eof()){
            break;
        }
        
        outputBoth = true;
        // std::cout << "I found this codeword : "<< codewordFound << std::endl; //debug
        auto decodedCodeword = dictionary.find(codewordFound);
        // std::cout << "Which maps to this string: " << decodedCodeword->second << std::endl;//debug
        nextByte = char(get_codeword(8, &bit_input));
        // std::cout << "and this byte : " << byte  << std::endl;//debug

        // std::cout << "I added " << codeword <<  "->" << decodedCodeword->second  << byte << " to the dictionary" << std::endl;//debug
        output << decodedCodeword->second << nextByte;

        
        dictionary[codeword] = decodedCodeword->second + nextByte;
        codeword+=1;
        
        // std::string decoded = dictionary[codewordFound];
        // std::string nextByte = 
        // std::cout << "decodedPard: " << decodedPart<< std::endl;
        // std::cout << " newChar " << newCharacter << std::endl;
        
    
    }
    if(!outputBoth){
        auto decodedCodeword = dictionary.find(codewordFound);
        std::cout  << decodedCodeword->second << std::endl;
        output << decodedCodeword->second;
        
    }

    std::cout << codewordFound << std::endl;
    std::cout << dictionary[codewordFound] << std::endl;

    std::cout << nextByte;
    CompressionStatistics* stats = new CompressionStatistics {true, 0.0, 0.0, 1, 1} ;
    return stats;
}

int main(){

    
   auto f = new LZW();
   std::cout << "Encoding File" << std::endl << std::endl;
   f->encode("./test/test.txt", "./test/encode2.txt");
   std::cout << std::endl << std::endl << "Decoding that Encoding" << std::endl << std::endl;
   f->decode("./test/encode2.txt", "./test/decode2.txt");
}