#pragma once
#include <string>
#include "bitio/bitio.hh"

class LZW {
  public:

    const int STARTING_DICT_SIZE=256;
    const int STARTING_CODE_SIZE=16;
    const int EOF_CODEWORD = 5;
    const int STARTING_CODEWORD = 6;

    typedef uint16_t codeword_type;

    LZW() = default;
    ~LZW() = default;

    void encode(const char* input_file, int file_size, std::ostream& output);
    void decode(const char* input, std::ostream& output);

};
