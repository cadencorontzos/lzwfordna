#pragma once
#include <string>
#include "bitio.hh"

class LZW {
  public:

    LZW();
    ~LZW();

    void encode(std::istream& input, std::ostream& output);
    void decode(std::istream& input, std::ostream& output);

};
