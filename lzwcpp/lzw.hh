#pragma once
#include <string>
#include "bitio.hh"

class LZW {
  public:

    LZW();
    ~LZW();

    void encode(std::ifstream& input, std::ofstream& output);
    void decode(std::ifstream& input, std::ofstream& output);

};
