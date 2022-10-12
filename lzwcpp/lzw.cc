#include "lzw.hh"
#include "bitio.hh"

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

    // if file can't be opened, move on to the next argument
    if (!input.is_open()){
        std::cout << "Unable to open " << filename << ".\n";
        return nullptr;
    }

    std::ofstream output;

    auto original_size =  fs::file_size(filename);

    output.open(output_filename, std::ios::binary);

    std::unordered_map<int, int> dictionary;

    for (int i = 0; i < 256; ++i){
        dictionary[i] = i;
    }

    int K;
    int codeword = 256;
    std::string str_K;
    int codeword_size = 8;
    int max_codeword_size = 8<<1;
    K = input.get();
    while (K != -1){
        

        // get returns -1 if EOF
        if (K == -1){
            break;
        }

        std::cout << char(K);
        K = input.get();
    }
    CompressionStatistics* stats = new CompressionStatistics {true, 0.0, 0.0, 1, 1} ;
    return stats;
}

    // Decode an input stream, and write to output stream
CompressionStatistics* LZW::decode(std::string filename, std::string output_filename){
    CompressionStatistics* stats = new CompressionStatistics {true, 0.0, 0.0, 1, 1} ;
    return stats;
}

int main(){

   std::cout << "Compling at least" << std::endl;
   auto f = new LZW();
   f->encode("./test/test.txt", "foo");
}