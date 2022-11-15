#include "lzw.hh"

#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]){

    if (argc != 2){
        std::cerr<<"Please include the name of the file you would like to compress\n";
        return 1;
    }

    std::ifstream input (argv[1], std::ios::binary);
    if (!input.is_open()){
        std::cout << "Unable to open " << argv[1] << ".\n";
        return 1;
    }

    LZW decompressor;
    std::ofstream output;

    output.open( std::string(argv[1])+".decompressed.lzw", std::ios::binary);
    
    // decompress file
    auto start_time = std::chrono::high_resolution_clock::now();
    decompressor.decode(input, output);
    auto end_time = std::chrono::high_resolution_clock::now();

    output.close();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // output statistics
    fs::path out =  std::string(argv[1])+".decompressed.lzw";
    auto output_size = fs::file_size(out);
    std::cout << "Decompressed file size (bytes) : " << output_size << std::endl;
    std::cout << "Decompression Time (ms) : " << duration.count() << std::endl;
    return 0;

}
