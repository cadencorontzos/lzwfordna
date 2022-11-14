#pragma once
#include <string>
#include "bitio.hh"

class LZW {
  public:

    const int STARTING_DICT_SIZE=256;
    const int STARTING_CODE_SIZE=9;
    const int EOF_CODEWORD = 256;
    const int STARTING_CODEWORD = 257;

    typedef uint64_t codeword_type;

    LZW() = default;
    ~LZW() = default;

    void encode(std::istream& input, std::ostream& output);
    void decode(std::istream& input, std::ostream& output);

};
