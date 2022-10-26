#pragma once
#include <string>

class LZW {
  public:
    static const int STARTING_CODE_SIZE = 8;
    static const int ALPHABET_SIZE = (1<<(STARTING_CODE_SIZE-1)); // (2^8)

    LZW();
    ~LZW();
    // Encode an input stream, and write to output stream
    void encode(std::ifstream& input, std::ofstream& output);

    // Decode an input stream, and write to output stream
    void decode(std::ifstream* input, std::ofstream* output);
};
