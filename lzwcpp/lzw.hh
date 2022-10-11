#pragma once
#include <string>

struct CompressionStatistics {
    bool compression;
    double compression_ratio;
    double compression_time;
    long original_file_size;
    long after_file_size;
    // void print(){
    //     std::cout << "Filename : " << std::endl;
    //     std::cout << "Original File Size: " << std::to_string(original_file_size) << std::endl;
    //     std::cout << "Compressed File Size: " << std::to_string(after_file_size) << std::endl;


    // }
};

class LZW {
  public:
    static const int STARTING_CODE_SIZE = 8;
    static const int ALPHABET_SIZE = (1<<(STARTING_CODE_SIZE-1)); // (2^8)

    LZW();
    ~LZW();
    // Encode an input stream, and write to output stream
    CompressionStatistics* encode(std::string filename, std::string output_filename);

    // Decode an input stream, and write to output stream
    CompressionStatistics* decode(std::string filename, std::string output_filename);
};
