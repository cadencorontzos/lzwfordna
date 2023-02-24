#pragma once
#include <string>
#include "bitio/bitio.hh"

class LZW {
  public:

    LZW() = default;
    ~LZW() = default;

    void encode(const char* input_file, int file_size, std::ostream& output);
    void decode(const char* input, std::ostream& output);

};
