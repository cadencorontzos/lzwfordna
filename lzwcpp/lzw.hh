#pragma once
#include <string>
#include "bitio.hh"

class LZW {
  public:

    const static int STARTING_DICT_SIZE=256;
    const static int STARTING_CODE_SIZE=9;
    const static int EOF_CODEWORD = 256;
    const static int STARTING_CODEWORD = 257;

    LZW();
    ~LZW();

    void encode(std::istream& input, std::ostream& output);
    void decode(std::istream& input, std::ostream& output);

};
