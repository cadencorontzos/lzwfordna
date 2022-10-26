#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;
#include "lzw.hh"

float compressionRatio( int output_size, int input_size){
    return (float)input_size/output_size;
}

int main(int argc, char* argv[]){
    // Checking that there are more than 1 arguments
    if (argc != 2){
        std::cerr<<"Please include the name of the file you would like to compress\n";
        return 1;
    }


    std::cout<<"Decompressing file "<<argv[1]<< std::endl;
   
    std::ifstream input (argv[1], std::ios::binary);

    if (!input.is_open()){
        std::cout << "Unable to open " << argv[1] << ".\n";
        return 1;
    }


    LZW compressor;
    std::ofstream output;
    fs::path inputFilePath = fs::current_path() / argv[1];

    output.open( std::string(argv[1])+".decompressed.lzw", std::ios::binary);

    auto start_time = std::chrono::high_resolution_clock::now();
    compressor.decode(input, output);
    auto end_time = std::chrono::high_resolution_clock::now();

    output.close();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // output statistics
    fs::path out =  std::string(argv[1])+".decompressed.lzw";
    auto output_size = fs::file_size(out);
    std::cout << "Decompressed file size: " << output_size << " bytes" << std::endl;
    std::cout << "Decompression Time: " << duration.count() << " ms" << std::endl;
    return 0;

}



// void reportTime()